// Copyright 2009-2013 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2013, Sandia Corporation
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _streamCPU_H
#define _streamCPU_H

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <sst/core/event.h>
#include <sst/core/sst_types.h>
#include <sst/core/component.h>
#include <sst/core/link.h>
#include <sst/core/timeConverter.h>

#include <sst/core/rng/marsaglia.h>
#include <sst/core/interfaces/memEvent.h>
using namespace SST::Interfaces;

namespace SST {
namespace MemHierarchy {

class streamCPU : public SST::Component {
public:

	streamCPU(SST::ComponentId_t id, SST::Component::Params_t& params);
	void init();
	void finish() {
		printf("streamCPU Finished after %"PRIu64" issued reads, %"PRIu64" returned\n",
				num_reads_issued, num_reads_returned);
		std::cout << "Completed @ " << getCurrentSimTimeNano() << " ns" << std::endl;
	}

private:
	streamCPU();  // for serialization only
	streamCPU(const streamCPU&); // do not implement
	void operator=(const streamCPU&); // do not implement
	void init(unsigned int phase);

	void handleEvent( SST::Event *ev );
	virtual bool clockTic( SST::Cycle_t );

    int numLS;
	int workPerCycle;
	int commFreq;
	bool do_write;
	uint32_t maxAddr;
	uint32_t nextAddr;
	uint64_t num_reads_issued, num_reads_returned;

	std::map<MemEvent::id_type, SimTime_t> requests;

	SST::Link* mem_link;

    SST::RNG::MarsagliaRNG rng;

    TimeConverter *clockTC;
    Clock::HandlerBase *clockHandler;

	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(workPerCycle);
		ar & BOOST_SERIALIZATION_NVP(commFreq);
		ar & BOOST_SERIALIZATION_NVP(maxAddr);
		ar & BOOST_SERIALIZATION_NVP(mem_link);
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Component);
		ar & BOOST_SERIALIZATION_NVP(workPerCycle);
		ar & BOOST_SERIALIZATION_NVP(commFreq);
		ar & BOOST_SERIALIZATION_NVP(maxAddr);
		ar & BOOST_SERIALIZATION_NVP(mem_link);
		//resture links
		mem_link->setFunctor(new SST::Event::Handler<streamCPU>(this,&streamCPU::handleEvent));
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

};

}
}
#endif /* _streamCPU_H */
