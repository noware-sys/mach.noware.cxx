#pragma once

#include <string>
#include <zmq/msg.hxx>

namespace noware
{
	namespace vmach
	{
		namespace cpu
		{
			class loader
				: virtual public mach::dev
			{
				public:
					virtual bool const load_file (std::string const &/* file_name*/, std::string /* thread_id*/ = "");
					virtual x86_64::opr const operation_get (std::string const &/* operation*/) const;
					
					
					// Store functionality
					
					virtual const std::string/* value*/ get (const std::string &/* group*/, const std::string &/* key*/) const;
					virtual const std::string/* value*/ get (const std::string &/* key*/) const;
					
					virtual const bool/* success*/ set (const std::string &/* group*/, const std::string &/* key*/, const std::string &/* content/value*/);
					virtual const bool/* success*/ set (const std::string &/* key*/, const std::string &/* content/value*/);
					
					
					// Queue functionality
					
					//virtual const bool enqueue (const cpu::x86_64::instr &);
					virtual bool const enqueue (std::string const &);
					
					
					// Device functionality
					
					virtual const bool/* success*/ search (zmq::msg &/* result*/, const zmq::msg &/* message/expression*/, const noware::nr &/* total, expected resonses count*/, const noware::nr &/* current count of peers who responded (so far)*/, const std::string &/* src*/, const net::cast &/* src_cast*/);// const
			};
		}
	}
}