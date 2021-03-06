const bool noware::mach::store::loader::load_file (const std::string & file_name, std::string const & thread_id)
{
	std::ifstream file;
	
	std::string thread;
	
	//cpu::x86_64::instr instr;
	std::string key, val;
	//unsigned long int ndx;
	//std::string token_prev;
	//std::string dest_offset_location, src_offset_location;
	
	// the first instruction;
	// the "entry point"
	//std::string instr1;
	
	file.open (file_name);
	
	if (!file.is_open ())
		return false;
	
	//ndx = 0;
	//instr.thread_id = noware::random::string (16);
	thread = std::string ("thread ") + thread_id;
	//token_prev = "";
	//_instr.thread_id = "1";
	//_instr.oprn = cpu::opr::set;
	//while (file >> operation >> dest >> dest_ref >> dest_is_offset >> dest_offset_location >> src >> src_ref >> src_is_offset >> src_offset_location)
	//while (file >> id >> device >> operation >> arg1type >> arg1 >> arg2type >> arg2 >> arg3type >> arg3 >> arg4type >> arg4)
	while (std::getline (file, key) && std::getline (file, val))
	//while (file >> token)
	{
		//++ndx;
		
		std::cerr << "set(\"" << thread << "\", \"" << key << "\", \"" << val << "\")" << std::endl;
		assert (set (thread, key, val));
		//assert (set (thread_id, std::string ("instr ") + label, _instr.serialize ()));
	}
	
	file.close ();
	
	//_loaded = true;
	
	return true;
}

const std::string/* value*/ noware::mach::store::loader::get (const std::string & group, const std::string & key) const
{
	std::map <std::string, std::string> expression;
	std::string expression_serial;
	
	expression ["subject"] = "obtainment";
	expression ["group"] = group;
	expression ["key"] = key;
	
	if (!noware::serialize <std::map <std::string, std::string>> (expression_serial, expression))
	{
		std::cerr << "noware::mach::store::loader::get()::serialize()::failure" << std::endl;
		
		return "";
	}
	
	return std::string (multival (zmq::msg (expression_serial), "noware::mach::store::nonempty"));
}

const std::string/* value*/ noware::mach::store::loader::get (const std::string & key) const
{
	return get (std::string (""), key);
}

std::string const/* value*/ noware::mach::store::loader::get (std::string const & group, noware::nr const & start, noware::nr const & size) const
{
	std::map <std::string, std::string> expression;
	
	expression ["subject"] = "obtainment";
	expression ["group"] = group;
	//expression ["key"] = key;
	
	expression ["type"] = "range";
	expression ["start"] = start.operator std::string const ();
	expression ["size"] = size.operator std::string const ();
	
	
	std::string expression_serial;
	if (!noware::serialize <std::map <std::string, std::string>> (expression_serial, expression))
		return "";
	
	return multival (zmq::msg (expression_serial), "noware::mach::store::nonempty");
}

std::string const/* value*/ noware::mach::store::loader::get (noware::nr const & start, noware::nr const & size) const
{
	return get ("", start, size);
}

const bool/* success*/ noware::mach::store::loader::set (const std::string & group, const std::string & key, const std::string & value)
{
	std::cerr << "noware::mach::store::loader::set()::in scope" << std::endl;
	
	std::map <std::string, std::string> expression;
	std::string expression_serial;
	
	expression ["subject"] = "assignment";
	expression ["group"] = group;
	expression ["key"] = key;
	expression ["value"] = value;
	
	if (!noware::serialize <std::map <std::string, std::string>> (expression_serial, expression))
	{
		std::cerr << "noware::mach::store::loader::set()::serialize()::false" << std::endl;
		
		return false;
	}
	
	//std::cerr << "noware::mach::store::loader::set()::return" << std::endl;
	if (std::string (anyval (zmq::msg (expression_serial), "noware::mach::store::nonfull")) == "1")
	{
		std::cerr << "noware::mach::store::loader::set()::return==[true]" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "noware::mach::store::loader::set()::return==[false]" << std::endl;
		return false;
	}
}

const bool/* success*/ noware::mach::store::loader::set (const std::string & key, const std::string & value)
{
	return set (std::string (""), key);
}

bool const noware::mach::store::loader::enqueue (std::string const & element)
{
	//return false;
	
	//if (full ())
	//	return false;
	
	//bool result;
	std::map <std::string, std::string> expression;
	std::string expression_serial;
	
	expression ["subject"] = "enqueue";
	expression ["element"] = element;
	
	if (!noware::serialize <std::map <std::string, std::string>> (expression_serial, expression))
		return false;
	
	if (std::string (anyval (zmq::msg (expression_serial), "noware::mach::queue::nonfull")) == "1")
	//if (std::string (multival (zmq::msg (expression_serial), "noware::mach::queue::nonfull")) == "1")
	{
		// Enqueuing notice.
		zmq::msg notice;
		std::string peer_id;
		
		notice = "enqueued(instr)";
		notice.prepend (zmq::msg::frame ("noware::mach::dev::response::nondesired"));	// Optional
		notice.prepend (zmq::msg::frame (noware::random::string (16)));
		
		// Try to transmit: "An instruction was enqueued.".
		std::cerr << "noware::mach::store::loader::enqueue()::broadcast" << std::endl;
		//node.multicast (notice, cpu::grp_dft);
		node.anycast (notice, peer_id, cpu::grp_dft);
		
		return true;
	}
	else
	{
		return false;
	}
}

const bool/* success*/ noware::mach::store::loader::search (zmq::msg & msg_result, const zmq::msg & msg_resp, const noware::nr &/* total, expected resonses count*/, const noware::nr &/* current count of peers who responded (so far)*/, const std::string &/* src*/, const net::cast &/* src_cast*/)
{
	std::cerr << "noware::mach::store::loader::search()::called" << std::endl;
	
	//noware::tree <std::string, std::string> resp;
	std::map <std::string, std::string> resp;
	//noware::tree <std::string, std::string> result;
	//std::string result;
	noware::var result_tmp;
	//bool result;
	
	//if (!resp.deserialize (msg_resp))
	if (!noware::deserialize <std::map <std::string, std::string>> (resp, std::string (msg_resp)))
	{
		std::cerr << "noware::mach::store::loader::search()::deserialize::failure" << std::endl;
		return false;
	}
	std::cerr << "noware::mach::store::loader::search()::deserialize::success" << std::endl;
	
	
	//result ["subject"] = resp ["subject"];
	
	// Store
	if (resp ["subject"] == "existence")
	{
		msg_result = resp ["value"];
		
		return msg_result == "1";
	}
	else if (resp ["subject"] == "obtainment")
	{
		if (resp ["value.exist"] == "1")
		{
			msg_result = resp ["value"];
			return true;
		}
		
		return false;
	}
	else if (resp ["subject"] == "assignment")
	{
		msg_result = resp ["value"];
		
		return msg_result == "1";
	}
	else if (resp ["subject"] == "removal")
	{
		msg_result = resp ["value"];
		
		//return msg_result == "1";
		return false;
	}
	else if (resp ["subject"] == "clearance")
	{
		msg_result = resp ["value"];
		
		//return msg_result == "1";
		return false;
	}
	// Queue
	else if (resp ["subject"] == "size::count")
	{
		std::cerr << "noware::mach::store::loader::search()::subject==[" << resp ["subject"] << ']' << std::endl;
		
		result_tmp = msg_result;
		//if (result_tmp.kind () != noware::var::type::nr)
		//	result_tmp = 0;
		
		//result += data.size ();
		//result ["value"] += resp ["value"];
		result_tmp += noware::var (resp ["value"]);
		msg_result = result_tmp;
		
		std::cerr << "noware::mach::store::loader::search()::result_tmp==[" << result_tmp << ']' << std::endl;
		std::cerr << "noware::mach::store::loader::search()::msg_result==[" << std::string (msg_result) << ']' << std::endl;
		
		return false;
	}
	else if (resp ["subject"] == "next_dequeue")
	{
		std::cerr << "noware::mach::store::loader::search()::subject==[" << resp ["subject"] << ']' << std::endl;
		
		//std::cerr << "noware::mach::store::loader::search()::queue.front ()==[" << queue.front () << ']' << std::endl;
		
		//result_tmp = queue.front ();
		//std::cerr << "noware::mach::store::loader::search()::result_tmp==[" << result_tmp << ']' << std::endl;
		
		
		if (resp ["success"] == "0")
			return false;
		
		//msg_result = result_tmp;
		//msg_result = queue.front ();
		msg_result = resp ["value"];
		std::cerr << "noware::mach::store::loader::search()::msg_result==[" << std::string (msg_result) << ']' << std::endl;
		
		return true;
		//return false;
		
		
		//std::cerr << "noware::mach::store::loader::search()::resp[success]==[" << resp ["success"] << ']' << std::endl;
		//return resp ["success"] == "1";
	}
	else if (resp ["subject"] == "next")
	{
		std::cerr << "noware::mach::store::loader::search()::subject==[" << resp ["subject"] << ']' << std::endl;
		
		//std::cerr << "noware::mach::store::loader::search()::queue.front ()==[" << queue.front () << ']' << std::endl;
		
		//result_tmp = queue.front ();
		//std::cerr << "noware::mach::store::loader::search()::result_tmp==[" << result_tmp << ']' << std::endl;
		
		
		if (resp ["success"] == "0")
			return false;
		
		//msg_result = result_tmp;
		//msg_result = queue.front ();
		msg_result = resp ["value"];
		std::cerr << "noware::mach::store::loader::search()::msg_result==[" << std::string (msg_result) << ']' << std::endl;
		
		return true;
		//return false;
		
		
		//std::cerr << "noware::mach::store::loader::search()::resp[success]==[" << resp ["success"] << ']' << std::endl;
		//return resp ["success"] == "1";
	}
	else if (resp ["subject"] == "dequeue")
	{
		std::cerr << "noware::mach::store::loader::search()::subject==[" << resp ["subject"] << ']' << std::endl;
		
		/*
		queue.pop ();
		
		if (empty_local ())
		{
			assert (node.leave ("noware::mach::queue::nonempty"));
		}
		
		assert (node.join ("noware::mach::queue::nonfull"));
		
		
		msg_result = "1";
		
		std::cerr << "noware::mach::queue::search()::result_tmp==[" << result_tmp << ']' << std::endl;
		std::cerr << "noware::mach::queue::search()::msg_result==[" << std::string (msg_result) << ']' << std::endl;
		
		return true;
		*/
		
		msg_result = resp ["value"];
		
		return msg_result == "1";
	}
	else if (resp ["subject"] == "enqueue")
	{
		std::cerr << "noware::mach::store::loader::search()::subject==[" << resp ["subject"] << ']' << std::endl;
		
		/*
		queue.push (resp ["element"]);
		
		if (full_local ())
		{
			assert (node.leave ("noware::mach::queue::nonfull"));
		}
		
		assert (node.join ("noware::mach::queue::nonempty"));
		
		
		msg_result = "1";
		
		std::cerr << "noware::mach::queue::search()::result_tmp==[" << result_tmp << ']' << std::endl;
		std::cerr << "noware::mach::queue::search()::msg_result==[" << std::string (msg_result) << ']' << std::endl;
		
		return true;
		*/
		
		msg_result = resp ["value"];
		
		return msg_result == "1";
	}
	
	return false;
}
