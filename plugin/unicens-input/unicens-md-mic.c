/*
 * Copyright (C) 2018, "IoT.bzh".
 * Author Jonathan Aillet
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

#include <stdio.h>
#include <string.h>

#include <wrap-json.h>

#include <ctl-config.h>

#include "unicens-md-mic.h"
#include "unicens-md-mic-cb.h"

CTLP_CAPI_REGISTER(UNICENS_MD_MIC_PLUGIN_NAME)

// Call at pre-initialisation time
CTLP_ONLOAD(plugin, callbacks)
{
	AFB_API_NOTICE(plugin->api, "%s Plugin Registered correctly: uid='%s' 'info='%s'", UNICENS_MD_MIC_PLUGIN_NAME, plugin->uid, plugin->info);

	return 0;
}

// Call at initialisation time
CTLP_INIT(plugin, callbacks)
{
	AFB_API_INFO(plugin->api, "Initialization of %s plugin", UNICENS_MD_MIC_PLUGIN_NAME);

	if(afb_api_require_api(plugin->api, UNICENS_API, 1)) {
		AFB_API_WARNING(plugin->api, "Didn't succeed to require %s api, won't be able to send command to unicens multi-directional microphone", UNICENS_API);
		return 0;
	}

	// Loading Unicens multi-directional microphone specific verbs
	if(afb_api_add_verb(plugin->api,
			    UNICENS_MD_MIC_SET_COLOR_SCHEME_VERB,
			    "Set Unicens multi-directional microphone color scheme",
			    UnicensMdMicSetColorScheme,
			    NULL,
			    NULL,
			    0,
			    0)) {
		AFB_API_ERROR(plugin->api,
			      "Error while creating verb for unicens multi-directional microphone plugin : '%s'",
			      UNICENS_MD_MIC_SET_COLOR_SCHEME_VERB);
		return -1;
	}

	if(afb_api_add_verb(plugin->api,
			    UNICENS_MD_MIC_SET_MODE_VERB,
			    "Set Unicens multi-directional microphone mode",
			    UnicensMdMicSetMode,
			    NULL,
			    NULL,
			    0,
			    0)) {
		AFB_API_ERROR(plugin->api,
			      "Error while creating verb for unicens multi-directional microphone plugin : '%s'",
			      UNICENS_MD_MIC_SET_MODE_VERB);
		return -2;
	}

	if(afb_api_add_verb(plugin->api,
			    UNICENS_MD_MIC_SET_DOA_VERB,
			    "Set Unicens multi-directional microphone DOA",
			    UnicensMdMicSetDoa,
			    NULL,
			    NULL,
			    0,
			    0)) {
		AFB_API_ERROR(plugin->api,
			      "Error while creating verb for unicens multi-directional microphone plugin : '%s'",
			      UNICENS_MD_MIC_SET_DOA_VERB);
		return -3;
	}

	if(afb_api_add_verb(plugin->api,
			    UNICENS_MD_MIC_GET_DOA_VERB,
			    "Get Unicens multi-directional microphone DOA",
			    UnicensMdMicGetDoa,
			    NULL,
			    NULL,
			    0,
			    0)) {
		AFB_API_ERROR(plugin->api,
			      "Error while creating verb for unicens multi-directional microphone plugin : '%s'",
			      UNICENS_MD_MIC_GET_DOA_VERB);
		return -4;
	}

	AFB_API_NOTICE(plugin->api, "Initialization of %s plugin correctly done", UNICENS_MD_MIC_PLUGIN_NAME);

	return 0;
}