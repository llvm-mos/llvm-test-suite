/*
 *  Phishing module: whitelist implementation.
 *
 *  Copyright (C) 2006-2007 T?r?k Edvin <edwin@clamav.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 *
 */

#if HAVE_CONFIG_H
#include "clamav-config.h"
#endif


#ifndef CL_DEBUG
#define NDEBUG
#endif

#ifdef CL_THREAD_SAFE
#ifndef _REENTRANT
#define _REENTRANT
#endif
#endif


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "clamav.h"
#include "others.h"
#include "phish_whitelist.h"
#include "regex_list.h"

int whitelist_match(const struct cl_engine* engine,char* real_url,const char* display_url,int hostOnly)
{
	const char* info;/*unused*/
	cli_dbgmsg("Phishing: looking up in whitelist: %s:%s; host-only:%d\n",real_url,display_url,hostOnly);
	return	engine->whitelist_matcher ? regex_list_match(engine->whitelist_matcher,real_url,display_url,NULL,hostOnly,&info,1) : 0;
}

int init_whitelist(struct cl_engine* engine)
{
	if(engine) {
		engine->whitelist_matcher = (struct regex_matcher *) cli_malloc(sizeof(struct regex_matcher));
		if(!engine->whitelist_matcher)
			return CL_EMEM;
		return	init_regex_list(engine->whitelist_matcher);
	}
	else
		return CL_ENULLARG;
}

int is_whitelist_ok(const struct cl_engine* engine)
{
	return (engine && engine->whitelist_matcher) ? is_regex_ok(engine->whitelist_matcher) : 1;
}

void whitelist_cleanup(const struct cl_engine* engine)
{
	if(engine && engine->whitelist_matcher) {
		regex_list_cleanup(engine->whitelist_matcher);
	}
}

void whitelist_done(struct cl_engine* engine)
{
	if(engine && engine->whitelist_matcher) {
		regex_list_done(engine->whitelist_matcher);	
		free(engine->whitelist_matcher);
		engine->whitelist_matcher = NULL;
	}
}

