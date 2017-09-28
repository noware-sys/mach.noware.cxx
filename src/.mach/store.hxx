#pragma once

// data store
// storage space
// remembers memories

class store
	: virtual public dev
	//, public misc::iterator <noware::var>
	, public contnr::array <std::string, std::string>
{
	public:
		//#include ".store/.hxx"
		
		store (void);
		virtual ~store (void);
		
		virtual const bool start (void);
		
		// Obtainers
		
		virtual const bool empty (void) const;
		virtual const bool empty_local (void) const;
		virtual const bool full (void) const;
		virtual const bool full_local (void) const;
		
		// size mag[nitude]
		// total size (available + used) (in bits)
		//const unsigned int magnitude (void) const;
		//const noware::nr::natural magnitude (void) const;
		virtual const noware::nr size (void) const;
		virtual const noware::nr size_local (void) const;
		
		// occ[upied] use[d]
		// used size (in bits)
		virtual const noware::nr used (void) const;
		virtual const noware::nr used_local (void) const;
		//const noware::nr used (const std::string &/* group*/) const;
		//const noware::nr used_local (const std::string &/* group*/) const;
		
		// free avail[able]
		// available size (in bits)
		virtual const noware::nr avail (void) const;
		virtual const noware::nr avail_local (void) const;
		
		// count
		// number of present elements
		virtual const noware::nr count (void) const;
		virtual const noware::nr count_local (void) const;
		virtual const noware::nr count (const std::string &/* group*/) const;
		virtual const noware::nr count_local (const std::string &/* group*/) const;
		
		// existence presence
		// exist present
		//const bool exist (const noware::var &/* group*/, const noware::var &/* key*/) const;
		virtual const bool exist (const std::string &/* key*/) const;
		virtual const bool exist (const std::string &/* group*/, const std::string &/* key*/) const;
		//const bool exist_group (const noware::var &/* group*/) const;
		
		// Returns the nodes which have a certain key.
		virtual const std::map <unsigned int/* map (array) index*/, std::string/* peer.id*/> owner (const std::string &/* key*/) const;
		virtual const std::map <unsigned int/* map (array) index*/, std::string/* peer.id*/> owner (const std::string &/* group*/, const std::string &/* key*/) const;
		
		// obtain attain get
		virtual const std::string/*value*/ get (const std::string &/* group*/, const std::string &/* key*/) const;
		virtual const std::string/*value*/ get (const std::string &/* key*/) const;
		//const std::pair <std::string/*value*/, bool/*reference*/> get (const std::string &/* group*/, const std::string &/* key*/) const;
		//const std::pair <std::string/*value*/, bool/*reference*/> get (const std::string &/* key*/) const;
		
		
		// Modifiers
		
		// reinitialize reset clear flush clean
		virtual const bool clear (void);
		virtual const bool clear (const std::string &/* group*/);
		
		// delete remove unset
		virtual const bool remove (const std::string &/* group*/, const std::string &/* key*/);
		virtual const bool remove (const std::string &/* key*/);
		
		// assign associate map set
		virtual const bool/* success*/ set (const std::string &/* group*/, const std::string &/* key*/, const std::string &/* content/value*/);
		virtual const bool/* success*/ set (const std::string &/* key*/, const std::string &/* content/value*/);
		//const bool/* success*/ set (const std::string &/* group*/, const std::string &/* key*/, const std::string &/* content/value*/, const bool &/* reference*/ = false);
		//const bool/* success*/ set (const std::string &/* key*/, const std::string &/* content/value*/, const bool &/* reference*/ = false);
		
		// Transmit a message to one node.
		//const noware::var unicast (const noware::tree <> &/* message*/, const std::string &/* peer*/);
		
		// Transmit a message to multiple nodes.
		// Main usage: To request other nodes to do an action.
		//const noware::var multicast (noware::tree <> /* message*/, const std::string &/* group*/ = group_default);
		
		//using device::evaluate;
		//static const noware::var group_zmq_default;
		static const std::string grp_dft;
	protected:
		// content value data container
		//noware::list <> data;
		//noware::list <noware::list <>> data;
		//noware::array <noware::array <>> data;
		
		std::map <std::string, std::map <std::string, std::string>> data;
		//std::map <std::string/*group*/, std::map <std::string/*key*/, std::pair <std::string/*value*/, bool/*reference*/>>> data;
	//public:
		// Manager of received messages.
		//void receive (const zyre_event_t */* zyre_event*/);
	//public:
	protected:
		//virtual const bool/* success*/ respond (const /*zmq::msg &*/zmsg_t */* message*/, const zyre_event_t */* (zyre) event*/);
		virtual const bool/* success*/ respond (zmq::msg &/* response*/, const zmq::msg &/* rx'd*/, const zyre_event_t */* (zyre) event*/, const std::string &/* event_type*/, const std::string &/* shouted_group*/, const net::cast &/* src_cast*/);
		//virtual const bool/* success*/ respond_post (const zyre_event_t */* (zyre) event*/, const std::string &/* event_type*/, const std::string &/* shouted_group*/, const zmq::msg &/* rx'd*/, const zmq::msg &/* response; read-only*/);
		//virtual const bool/* success*/ infrastruct (const zyre_event_t */* (zyre) event*/, const std::string &/* event_type*/, const zmq::msg &/* rx'd*/);
		virtual const bool/* success*/ search (zmq::msg &/* result*/, const zmq::msg &/* message/expression*/, const std::string &/* src*/, const net::cast &/* src_cast*/);// const
		virtual const bool/* success*/ search_local (zmq::msg &/* result*/, const zmq::msg &/* message/expression*/, const std::string &/* src*/, const net::cast &/* src_cast*/);// const
		virtual const zmq::msg/* result*/ aggregate (const zmq::msg &/* result*/, const zmq::msg &/* response*/, const zmq::msg &/* expression*/, const noware::nr &/* responses_count*//* number of peers who answered*/, const std::string &/* src*/, const net::cast &/* src_cast*/);
	public:
		bool writable;
		/*
			// Iteration-related:
			//virtual const iterator <value> begin (void) const;
			//virtual const iterator <value> end (void) const;
			//
			virtual const iterator <value> operator ++ (void);
			virtual const iterator <value> operator -- (void);
			
			virtual const bool operator == (const iterator <value> &) const;
			virtual const value operator * (void) const;
		*/
		
	//protected:
		// Receives responses to queries.
		//const std::string receive_local (const std::string &/* response_type*/, const std::string &/* filter*/ = "") const;
		
		// Transmits responses to queries.
		//const bool unicast_local (const noware::tree <> &/* message*/) const;
	//protected:
		//const bool search_exists (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_get (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_set (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_remove (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_magnitude (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_clear (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
		//const bool search_empty (const noware::tree <> &/* message*/, noware::var &/* result*/);// const;
};

//#include "storage.cxx"
