/*
 * Copyright (C) 2018, "IoT.bzh", Microchip Technology Inc. and its subsidiaries.
 * Author Jonathan Aillet
 * Author Tobias Jahnke
 * Contrib Fulup Ar Foll
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include <wrap-json.h>
#include <ctl-plugin.h>
#include <ctl-config.h>

#include "wrap_unicens.h"
#include "wrap_volume.h"

#define PCM_MAX_CHANNELS    6
#define UCS_PLUGIN_NAME     "hal-unicens"

CTLP_CAPI_REGISTER(UCS_PLUGIN_NAME)
        
AFB_ApiT unicensHalApiHandle;
static uint8_t initialized = 0;


/* Call at initialization time, binder is not running yet,
 * communication to other bindings not possible now. */
CTLP_ONLOAD(plugin, callbacks)
{
    unicensHalApiHandle = plugin->api;
    AFB_ApiNotice(unicensHalApiHandle, "4A-HAL-UNICENS: Plugin Register: uid='%s' 'info='%s'", plugin->uid, plugin->info);
    return 0;
}


/* initializes ALSA sound card, UNICENS API */
CTLP_CAPI(Init, source, argsJ, queryJ)
{
    int err = 0;
    int pcm_volume[PCM_MAX_CHANNELS] = { 100, 100, 100, 100, 100, 100 };

    AFB_ApiNotice(source->api, "4A-HAL-UNICENS: Initializing 4a plugin");

    if((err = wrap_volume_init())) {
            AFB_ApiError(source->api, "Failed to initialize wrapper for volume library");
            goto Abort_Exit;
    }

    if((err = wrap_ucs_subscribe_sync(source->api))) {
            AFB_ApiError(source->api, "Failed to subscribe to UNICENS binding");
            goto Abort_Exit;
    }

    // Set output volume to pre-defined level in order to
    // avoid muted volume to be persistent after boot.
    //wrap_volume_master(source->api, 80);
    wrap_volume_pcm(source->api, pcm_volume, PCM_MAX_CHANNELS);
    initialized = 1;

Abort_Exit:
    AFB_ApiNotice(source->api, "4A-HAL-UNICENS: Initializing plugin done, err=%d", err);
    err = 0;            /* avoid returning non-zero value, to force loading the plugin */
    return err;
}


/* Is called when master volume is incremented by ALSA */
CTLP_CAPI(MasterVol, source, argsJ, queryJ)
{
    int master_volume;
    json_object *valueJ;
    int err = 0;

    AFB_ApiNotice(source->api, "4A-HAL-UNICENS: MasterVolume=%s", json_object_to_json_string(queryJ));
    if(! initialized) {
            AFB_ApiWarning(source->api, "%s: Link to unicens binder is not initialized, can't set master volume, value=%s", __func__, json_object_get_string(queryJ));
            err = -1;
            goto Abort_Exit;
    }

    if(! json_object_is_type(queryJ, json_type_array) || json_object_array_length(queryJ) <= 0) {
            AFB_ApiError(source->api, "%s: invalid json (should be a non empty json array) value=%s", __func__, json_object_get_string(queryJ));
            err = -2;
            goto Abort_Exit;
    }

    valueJ = json_object_array_get_idx(queryJ, 0);
    if(! json_object_is_type(valueJ, json_type_int)) {
            AFB_ApiError(source->api, "%s: invalid json (should be an array of int) value=%s", __func__, json_object_get_string(queryJ));
            err = -3;
            goto Abort_Exit;
    }

    master_volume = json_object_get_int(valueJ);
    wrap_volume_master(source->api, master_volume);

Abort_Exit:        
    err = 0;            /* avoid returning non-zero value, to force loading the plugin */
    return err;
}


/* not used at the moment, disabled in JSON configuration */
CTLP_CAPI(PCMVol, source, argsJ, queryJ)
{
    AFB_ApiNotice(source->api, "4A-HAL-UNICENS: PCMVolume=%s", json_object_to_json_string(queryJ));
    return 0;
}


/* Is called on UNICENS events */
CTLP_CAPI(Events, source, argsJ, queryJ)
{
    uint16_t node = 0U;
    bool available = false;
    int err = 0;
    json_object *j_tmp = NULL;
    
    AFB_ApiError(source->api, "4A-HAL-UNICENS: receiving event query=%s", json_object_to_json_string(queryJ));

    if (initialized == 0) {
        AFB_ApiError(source->api, "4A-HAL-UNICENS: Not initialized while receiving event query=%s", json_object_to_json_string(queryJ));
        err = 0;
        goto Abort_Exit;
    }

    if (json_object_object_get_ex(queryJ, "node", &j_tmp)) {
        node = (uint16_t)json_object_get_int(j_tmp);
    }
    else {
        err = -1;
    }

    if (json_object_object_get_ex(queryJ, "available", &j_tmp)) {
        available = (bool)json_object_get_boolean(j_tmp);
    }
    else {
        err = -2;
    }

    if(err == 0) {
        AFB_ApiNotice(source->api, "4A-HAL-UNICENS: Node-Availability: node=0x%03X, available=%d", node, available);
        wrap_volume_node_avail(source->api, node, available);
    }
    else {
        AFB_ApiError(source->api, "4A-HAL-UNICENS: Failed to parse events query=%s", json_object_to_json_string(queryJ));
    }

Abort_Exit:
    err = 0;            /* avoid returning non-zero value now */
    return err;
}
