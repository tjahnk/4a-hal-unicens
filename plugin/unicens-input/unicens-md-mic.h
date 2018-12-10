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

#ifndef _UNICENS_MD_MIC_INCLUDE_
#define _UNICENS_MD_MIC_INCLUDE_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define UNICENS_MD_MIC_PLUGIN_NAME			"unicens-md-input"

#define UNICENS_API					"UNICENS"
#define UNICENS_SEND_MSG_VERB				"sendmessage"

#define UNICENS_MD_MIC_NODE				0x520

#define UNICENS_MD_MIC_SET_COLOR_SCHEME_MSG_ID		0x1000
#define UNICENS_MD_MIC_SET_MODE_MSG_ID			0x1001
#define UNICENS_MD_MIC_SET_DOA_MSG_ID			0x1002
#define UNICENS_MD_MIC_GET_DOA_MSG_ID			0x1003

#define UNICENS_MD_MIC_SET_COLOR_SCHEME_CMD		0x00
#define UNICENS_MD_MIC_SET_COLOR_SCHEME_DATA_LENGTH	10

#define UNICENS_MD_MIC_SET_MODE_CMD			0x00
#define UNICENS_MD_MIC_SET_MODE_OFF			0x00
#define UNICENS_MD_MIC_SET_MODE_IDLE			0x01
#define UNICENS_MD_MIC_SET_MODE_LISTENING		0x02
#define UNICENS_MD_MIC_SET_MODE_PROCESSING		0x03
#define UNICENS_MD_MIC_SET_MODE_ERROR			0x04
#define UNICENS_MD_MIC_SET_MODE_DOA			0x06
#define UNICENS_MD_MIC_SET_MODE_DATA_LENGTH		2

#define UNICENS_MD_MIC_SET_DOA_CMD			0x00
#define UNICENS_MD_MIC_SET_DOA_DATA_LENGTH		3

#define UNICENS_MD_MIC_GET_DOA_DATA			0x01
#define UNICENS_MD_MIC_GET_DOA_DATA_LENGTH		1

#endif /*_UNICENS_MD_MIC_INCLUDE_ */