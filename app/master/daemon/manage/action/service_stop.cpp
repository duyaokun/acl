/**
 * Copyright (C) 2015-2018
 * All rights reserved.
 *
 * AUTHOR(S)
 *   Zheng Shuxin
 *   E-mail: zhengshuxin@qiyi.com
 * 
 * VERSION
 *   Fri 16 Jun 2017 01:58:08 PM CST
 */

#include "stdafx.h"
#include "master/master_api.h"
#include "service_stop.h"

bool service_stop::stop_one(const char* path, stop_res_data_t& data)
{
	if (acl_master_stop(path) < 0)
	{
		data.status = 404;
		return false;
	}

	data.status = 200;
	return true;
}

bool service_stop::run(const stop_req_t& req, stop_res_t& res)
{
	size_t n = 0;

	for (std::vector<stop_req_data_t>::const_iterator
		cit = req.data.begin(); cit != req.data.end(); ++cit)
	{
		stop_res_data_t data;
		data.path = (*cit).path;

		if (stop_one((*cit).path.c_str(), data))
			n++;
		res.data.push_back(data);
	}

	if (n == req.data.size())
	{
		res.status = 200;
		res.msg    = "ok";
	}
	else
	{
		res.status = 500;
		res.msg    = "error";
		logger_error("not all services were started!, n=%d, %d",
			(int) n, (int) req.data.size());
	}

	return true;
}
