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

#include <afb/afb-binding.h>

#include "unicens-md-mic.h"
#include "unicens-md-mic-cb.h"

/*******************************************************************************
 *	Unicens multi-directional microphone plugin verbs functions	       *
 ******************************************************************************/

void UnicensMdMicSetColorScheme(afb_req_t request)
{
	int32_t darkRed, darkBlue, darkGreen, lightRed, lightBlue, lightGreen, errorRed, errorBlue, errorGreen;

	uint8_t setColorSchemeData[UNICENS_MD_MIC_SET_COLOR_SCHEME_DATA_LENGTH];

	char *error, *info;

	json_object *receivedRequestJson, *unicensServiceRequestJson, *unicensServiceReturnedJson;

	afb_api_t apiHandle;

	if(! (receivedRequestJson = afb_req_json(request))) {
		afb_req_fail(request, "request_json", "Can't get request json");
		return;
	}

	if(! (apiHandle = (afb_api_t ) afb_req_get_api(request))) {
		afb_req_fail(request, "api_handle", "Can't get current hal controller api handle");
		return;
	}

	if(wrap_json_unpack(receivedRequestJson, "{s:{s:i, s:i, s:i}, s:{s:i, s:i, s:i}, s:{s:i, s:i, s:i}}",
						 "dark",
						 "red", &darkRed,
						 "green", &darkGreen,
						 "blue", &darkBlue,
						 "light",
						 "red", &lightRed,
						 "green", &lightGreen,
						 "blue", &lightBlue,
						 "error",
						 "red", &errorRed,
						 "green", &errorGreen,
						 "blue", &errorBlue)) {
		afb_req_fail_f(request, "requested_colors", "Can't get requested colors, might be a syntax error (json format should be : '%s')", UNICENS_MD_MIC_SET_COLOR_SCHEME_JSON);
		return;
	}

	if(darkRed < 0 || darkRed > 255 || darkGreen < 0 || darkGreen > 255 || darkBlue < 0 || darkBlue > 255 ||
	   lightRed < 0 || lightRed > 255 || lightGreen < 0 || lightGreen > 255 || lightBlue < 0 || lightBlue > 255 ||
	   errorRed < 0 || errorRed > 255 || errorGreen < 0 || errorGreen > 255 || errorBlue < 0 || errorBlue > 255) {
		afb_req_fail(request, "requested_colors", "Invalid requested colors, colors should be between 0 and 255");
		return;
	}

	setColorSchemeData[0] = (uint8_t) UNICENS_MD_MIC_SET_COLOR_SCHEME_CMD;
	setColorSchemeData[1] = (uint8_t) darkRed;
	setColorSchemeData[2] = (uint8_t) darkGreen;
	setColorSchemeData[3] = (uint8_t) darkBlue;
	setColorSchemeData[4] = (uint8_t) lightRed;
	setColorSchemeData[5] = (uint8_t) lightGreen;
	setColorSchemeData[6] = (uint8_t) lightBlue;
	setColorSchemeData[7] = (uint8_t) errorRed;
	setColorSchemeData[8] = (uint8_t) errorGreen;
	setColorSchemeData[9] = (uint8_t) errorBlue;

	wrap_json_pack(&unicensServiceRequestJson, "{s:i, s:i, s:y}",
						   "node", UNICENS_MD_MIC_NODE,
						   "msgid", UNICENS_MD_MIC_SET_COLOR_SCHEME_MSG_ID,
						   "data", setColorSchemeData, (size_t) UNICENS_MD_MIC_SET_COLOR_SCHEME_DATA_LENGTH);

	if(afb_api_call_sync(apiHandle,
			     UNICENS_API,
			     UNICENS_SEND_MSG_VERB,
			     unicensServiceRequestJson,
			     &unicensServiceReturnedJson,
			     &error,
			     &info)) {
		afb_req_fail_f(request,
			       "unicens_service_call",
			       "Error '%s' during call to '%s' verb of '%s' api (info: '%s')",
			       error, UNICENS_SEND_MSG_VERB, UNICENS_API, info);
		return;
	}

	afb_req_success_f(request, NULL, "Set color scheme correctly send on MOST network, multi-directional microphone colors should be set according to your request");
}

void UnicensMdMicSetMode(afb_req_t request)
{
	uint8_t setModeData[UNICENS_MD_MIC_SET_MODE_DATA_LENGTH];

	char *mode, *error, *info;

	json_object *receivedRequestJson, *unicensServiceRequestJson, *unicensServiceReturnedJson;

	afb_api_t apiHandle;

	if(! (receivedRequestJson = afb_req_json(request))) {
		afb_req_fail(request, "request_json", "Can't get request json");
		return;
	}

	if(! (apiHandle = (afb_api_t ) afb_req_get_api(request))) {
		afb_req_fail(request, "api_handle", "Can't get current hal controller api handle");
		return;
	}

	if(wrap_json_unpack(receivedRequestJson, "{s:s}", "mode", &mode)) {
		afb_req_fail(request, "requested_mode", "Can't get requested mode, might be a syntax error (mode should be a string)");
		return;
	}

	setModeData[0] = (uint8_t) UNICENS_MD_MIC_SET_COLOR_SCHEME_CMD;

	if(! strcasecmp(mode, "off")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_OFF;
	}
	else if(! strcasecmp(mode, "idle")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_IDLE;
	}
	else if(! strcasecmp(mode, "listening")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_LISTENING;
	}
	else if(! strcasecmp(mode, "processing")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_PROCESSING;
	}
	else if(! strcasecmp(mode, "error")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_ERROR;
	}
	else if(! strcasecmp(mode, "doa")) {
		setModeData[1] = (uint8_t) UNICENS_MD_MIC_SET_MODE_DOA;
	}
	else {
		afb_req_fail(request, "requested_mode", "Invalid requested mode, should be : 'off', 'idle', 'listening', 'processing', 'error', or 'doa'");
		return;
	}

	wrap_json_pack(&unicensServiceRequestJson, "{s:i, s:i, s:y}",
						   "node", UNICENS_MD_MIC_NODE,
						   "msgid", UNICENS_MD_MIC_SET_MODE_MSG_ID,
						   "data", setModeData, (size_t) UNICENS_MD_MIC_SET_MODE_DATA_LENGTH);

	if(afb_api_call_sync(apiHandle,
			     UNICENS_API,
			     UNICENS_SEND_MSG_VERB,
			     unicensServiceRequestJson,
			     &unicensServiceReturnedJson,
			     &error,
			     &info)) {
		afb_req_fail_f(request,
			       "unicens_service_call",
			       "Error '%s' during call to '%s' verb of '%s' api (info: '%s')",
			       error, UNICENS_SEND_MSG_VERB, UNICENS_API, info);
		return;
	}

	afb_req_success_f(request, NULL, "Set mode correctly send on MOST network, multi-directional microphone mode should be set according to your request");
}

void UnicensMdMicSetDoa(afb_req_t request)
{
	uint8_t setDoaData[UNICENS_MD_MIC_SET_DOA_DATA_LENGTH];

	int32_t doa;

	char *error, *info;

	json_object *receivedRequestJson, *unicensServiceRequestJson, *unicensServiceReturnedJson;

	afb_api_t apiHandle;

	if(! (receivedRequestJson = afb_req_json(request))) {
		afb_req_fail(request, "request_json", "Can't get request json");
		return;
	}

	if(! (apiHandle = (afb_api_t ) afb_req_get_api(request))) {
		afb_req_fail(request, "api_handle", "Can't get current hal controller api handle");
		return;
	}

	if(wrap_json_unpack(receivedRequestJson, "{s:i}", "doa", &doa)) {
		afb_req_fail(request, "requested_doa", "Can't get requested doa (doa should be an integer)");
		return;
	}

	if(doa < 0 || doa > 359) {
		afb_req_fail(request, "requested_doa", "Invalid doa, should be expressed in degree between 0 and 359");
		return;
	}

	setDoaData[0] = (uint8_t) UNICENS_MD_MIC_SET_DOA_CMD;
	setDoaData[1] = (uint8_t) ((doa & 0xFF00) >> 8);
	setDoaData[2] = (uint8_t) (doa & 0xFF);

	wrap_json_pack(&unicensServiceRequestJson, "{s:i, s:i, s:y}",
						   "node", UNICENS_MD_MIC_NODE,
						   "msgid", UNICENS_MD_MIC_SET_DOA_MSG_ID,
						   "data", setDoaData, (size_t) UNICENS_MD_MIC_SET_DOA_DATA_LENGTH);

	if(afb_api_call_sync(apiHandle,
			     UNICENS_API,
			     UNICENS_SEND_MSG_VERB,
			     unicensServiceRequestJson,
			     &unicensServiceReturnedJson,
			     &error,
			     &info)) {
		afb_req_fail_f(request,
			       "unicens_service_call",
			       "Error '%s' during call to '%s' verb of '%s' api (info: '%s')",
			       error, UNICENS_SEND_MSG_VERB, UNICENS_API, info);
		return;
	}

	afb_req_success_f(request, NULL, "Set doa correctly send on MOST network, multi-directional microphone doa should be set according to your request");
}

void UnicensMdMicGetDoa(afb_req_t request)
{
	AFB_REQ_WARNING(request, "Not implemented because %s api does not include the doa with its response", UNICENS_API);

	afb_req_success_f(request, NULL, "Not implemented because %s api does not include the doa with its response", UNICENS_API);
}