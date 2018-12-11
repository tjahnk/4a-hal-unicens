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

#ifndef _UNICENS_MD_MIC_CB_INCLUDE_
#define _UNICENS_MD_MIC_CB_INCLUDE_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <afb/afb-binding.h>

#define UNICENS_MD_MIC_SET_COLOR_SCHEME_VERB	"set_md_mic_color_scheme"
#define UNICENS_MD_MIC_SET_MODE_VERB		"set_md_mic_mode"
#define UNICENS_MD_MIC_SET_DOA_VERB		"set_md_mic_doa"
#define UNICENS_MD_MIC_GET_DOA_VERB		"get_md_mic_doa"

#define UNICENS_MD_MIC_SET_COLOR_SCHEME_JSON	"\
\"light\" : \
{ \
\"red\" : integer, \
\"green\" : integer, \
\"blue\" : integer \
}, \
\"dark\" : \
{ \
\"red\" : integer, \
\"green\" : integer, \
\"blue\" : integer \
}, \
\"error\" : \
{ \
\"red\" : integer, \
\"green\" : integer, \
\"blue\" : integer \
} \
}\
"

// Unicens multi-directional microphone plugin verbs functions
void UnicensMdMicSetColorScheme(afb_req_t request);
void UnicensMdMicSetMode(afb_req_t request);
void UnicensMdMicSetDoa(afb_req_t request);
void UnicensMdMicGetDoa(afb_req_t request);

#endif /*_UNICENS_MD_MIC_CB_INCLUDE_ */