/*
** $Id: sst_gen.c,v 1.13 2010/05/13 19:27:23 rolf Exp $
**
** Rolf Riesen, March 2010, Sandia National Laboratories
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sst_gen_v2.h"
#include "gen.h"

#define MAX_ID_LEN		(256)


/* Number of messages in the message rate pattern */
#define MSGRATE_NUM_MSGS	(200)



void
sst_header(FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "<?xml version=\"2.0\"?>\n");
    fprintf(sstfile, "\n");

    /*
    ** For now we do the config section here. This may have to be
    ** broken out later, if we actually start config values.
    */
    fprintf(sstfile, "<config>\n");
    fprintf(sstfile, "\trun-mode=both\n");
    fprintf(sstfile, "</config>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_header() */



void
sst_variables(FILE *sstfile, uint64_t node_latency, uint64_t net_latency)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "<variables>\n");
    fprintf(sstfile, "\t<lat_local_net> %luns </lat_local_net>\n", node_latency);
    fprintf(sstfile, "\t<lat_global_net> %luns </lat_global_net>\n", net_latency);
    fprintf(sstfile, "\t<lat_local_nvram> %luns </lat_local_nvram>\n", node_latency);
    fprintf(sstfile, "\t<lat_storage_net> %luns </lat_storage_net>\n", net_latency);
    fprintf(sstfile, "\t<lat_storage_nvram> %luns </lat_storage_nvram>\n", node_latency);
    fprintf(sstfile, "\t<lat_ssd_io> %luns </lat_ssd_io>\n", net_latency);
    fprintf(sstfile, "</variables>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_variables() */



void
sst_param_start(FILE *sstfile)
{

    if (sstfile != NULL)   {
	fprintf(sstfile, "<param_include>\n");
    }

}  /* end of sst_param_start() */



void
sst_param_end(FILE *sstfile)
{

    if (sstfile != NULL)   {
	fprintf(sstfile, "</param_include>\n\n");
    }

}  /* end of sst_param_end() */



void
sst_gen_param_start(FILE *sstfile, int gen_debug)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t<Gp>\n");
    fprintf(sstfile, "\t\t<debug> %d </debug>\n", gen_debug);

}  /* end of sst_gen_param_start() */


void
sst_gen_param_entries(FILE *sstfile, int x_dim, int y_dim, int NoC_x_dim, int NoC_y_dim,
	int cores, int nodes, uint64_t net_lat,
        uint64_t net_bw, uint64_t node_lat, uint64_t node_bw, uint64_t compute_time,
	uint64_t app_time, int msg_len, char *method,
	uint64_t chckpt_interval, int envelope_size, int chckpt_size,
	char *pattern_name)
{

    if (sstfile == NULL)   {
	return;
    }

    /* Common parameters */
    fprintf(sstfile, "\t\t<x_dim> %d </x_dim>\n", x_dim);
    fprintf(sstfile, "\t\t<y_dim> %d </y_dim>\n", y_dim);
    fprintf(sstfile, "\t\t<NoC_x_dim> %d </NoC_x_dim>\n", NoC_x_dim);
    fprintf(sstfile, "\t\t<NoC_y_dim> %d </NoC_y_dim>\n", NoC_y_dim);
    fprintf(sstfile, "\t\t<cores> %d </cores>\n", cores);
    fprintf(sstfile, "\t\t<nodes> %d </nodes>\n", cores);
    fprintf(sstfile, "\t\t<net_latency> %lu </net_latency>\n", net_lat);
    fprintf(sstfile, "\t\t<net_bandwidth> %lu </net_bandwidth>\n", net_bw);
    fprintf(sstfile, "\t\t<node_latency> %lu </node_latency>\n", node_lat);
    fprintf(sstfile, "\t\t<node_bandwidth> %lu </node_bandwidth>\n", node_bw);

    fprintf(sstfile, "\t\t<exchange_msg_len> %d </exchange_msg_len>\n", msg_len);
    fprintf(sstfile, "\t\t<envelope_size> %d </envelope_size>\n", envelope_size);


    /* Pattern specific parameters */
    if (strcmp("ghost_pattern", pattern_name) == 0)   {
	fprintf(sstfile, "\t\t<compute_time> %lu </compute_time>\n", compute_time);
	fprintf(sstfile, "\t\t<application_end_time> %lu </application_end_time>\n", app_time);
	fprintf(sstfile, "\t\t<chckpt_method> %s </chckpt_method>\n", method);
	fprintf(sstfile, "\t\t<chckpt_interval> %lu </chckpt_interval>\n", chckpt_interval);
	fprintf(sstfile, "\t\t<chckpt_size> %d </chckpt_size>\n", chckpt_size);
    }

    if (strcmp("msgrate_pattern", pattern_name) == 0)   {
	fprintf(sstfile, "\t\t<num_msgs> %d </num_msgs>\n", MSGRATE_NUM_MSGS);
    }

}  /* end of sst_gen_param_entries() */


void
sst_gen_param_end(FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t</Gp>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_gen_param_end() */


void
sst_pwr_param_entries(FILE *sstfile, pwr_method_t power_method)
{
    if (sstfile == NULL)   {
	return;
    }

    if (power_method == pwrNone)   {
	/* Nothing to do */
	return;
    } else if (power_method == pwrMcPAT)   {
	fprintf(sstfile, "\t<intro1_params>\n");
	fprintf(sstfile, "\t\t<period>15000000ns</period>\n");
	fprintf(sstfile, "\t\t<model>routermodel_power</model>\n");
	fprintf(sstfile, "\t</intro1_params>\n");
	fprintf(sstfile, "\n");
    } else if (power_method == pwrORION)   {
	fprintf(sstfile, "\t<intro1_params>\n");
	fprintf(sstfile, "\t\t<period>15000000ns</period>\n");
	fprintf(sstfile, "\t\t<model>routermodel_power</model>\n");
	fprintf(sstfile, "\t</intro1_params>\n");
	fprintf(sstfile, "\n");
    } else   {
	/* error */
    }

}  /* end of sst_pwr_param_entries() */


void
sst_nvram_param_entries(FILE *sstfile, int nvram_read_bw, int nvram_write_bw,
	int ssd_read_bw, int ssd_write_bw)
{
    if (sstfile == NULL)   {
	return;
    }

    fprintf(sstfile, "\t<NVRAMparams>\n");
    fprintf(sstfile, "\t\t<debug> 0 </debug>\n");
    fprintf(sstfile, "\t\t<read_bw> %d </read_bw>\n", nvram_read_bw);
    fprintf(sstfile, "\t\t<write_bw> %d </write_bw>\n", nvram_write_bw);
    fprintf(sstfile, "\t</NVRAMparams>\n");
    fprintf(sstfile, "\n");

    fprintf(sstfile, "\t<SSDparams>\n");
    fprintf(sstfile, "\t\t<debug> 0 </debug>\n");
    fprintf(sstfile, "\t\t<read_bw> %d </read_bw>\n", ssd_read_bw);
    fprintf(sstfile, "\t\t<write_bw> %d </write_bw>\n", ssd_write_bw);
    fprintf(sstfile, "\t</SSDparams>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_nvram_param_entries() */


void
sst_router_param_start(FILE *sstfile, char *Rname, int num_ports, uint64_t router_bw, int num_cores,
    int hop_delay, int wormhole, pwr_method_t power_method)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t<%s>\n", Rname);
    fprintf(sstfile, "\t\t<hop_delay> %d </hop_delay>\n", hop_delay);
    fprintf(sstfile, "\t\t<debug> 0 </debug>\n");
    fprintf(sstfile, "\t\t<num_ports> %d </num_ports>\n", num_ports);
    fprintf(sstfile, "\t\t<bw> %lu </bw>\n", router_bw);
    fprintf(sstfile, "\t\t<wormhole> %d </wormhole>\n", wormhole);

    if (power_method == pwrNone)   {
	/* Nothing to do */
    } else if (power_method == pwrMcPAT)   {
	fprintf(sstfile, "\t\t<McPAT_XMLfile>../../core/techModels/libMcPATbeta/Niagara1.xml</McPAT_XMLfile>\n");
	fprintf(sstfile, "\t\t<core_temperature>350</core_temperature>\n");
	fprintf(sstfile, "\t\t<core_tech_node>65</core_tech_node>\n");
	fprintf(sstfile, "\t\t<frequency>1ns</frequency>\n");
	fprintf(sstfile, "\t\t<power_monitor>YES</power_monitor>\n");
	fprintf(sstfile, "\t\t<temperature_monitor>NO</temperature_monitor>\n");
	fprintf(sstfile, "\t\t<router_voltage>1.1</router_voltage>\n");
	fprintf(sstfile, "\t\t<router_clock_rate>1000000000</router_clock_rate>\n");
	fprintf(sstfile, "\t\t<router_flit_bits>64</router_flit_bits>\n");
	fprintf(sstfile, "\t\t<router_virtual_channel_per_port>2</router_virtual_channel_per_port>\n");
	fprintf(sstfile, "\t\t<router_input_ports>%d</router_input_ports>\n", num_ports);
	fprintf(sstfile, "\t\t<router_output_ports>%d</router_output_ports>\n", num_ports);
	fprintf(sstfile, "\t\t<router_link_length>16691</router_link_length>\n");
	fprintf(sstfile, "\t\t<router_power_model>McPAT</router_power_model>\n");
	fprintf(sstfile, "\t\t<router_has_global_link>1</router_has_global_link>\n");
	fprintf(sstfile, "\t\t<router_input_buffer_entries_per_vc>16</router_input_buffer_entries_per_vc>\n");
	fprintf(sstfile, "\t\t<router_link_throughput>1</router_link_throughput>\n");
	fprintf(sstfile, "\t\t<router_link_latency>1</router_link_latency>\n");
	fprintf(sstfile, "\t\t<router_horizontal_nodes>1</router_horizontal_nodes>\n");
	fprintf(sstfile, "\t\t<router_vertical_nodes>1</router_vertical_nodes>\n");
	fprintf(sstfile, "\t\t<router_topology>RING</router_topology>\n");
	fprintf(sstfile, "\t\t<core_number_of_NoCs>%d</core_number_of_NoCs>\n", num_cores);
	fprintf(sstfile, "\t\t<push_introspector>routerIntrospector</push_introspector>\n");

    } else if (power_method == pwrORION)   {
	fprintf(sstfile, "\t\t<ORION_XMLfile>../../core/techModels/libORION/something.xml</ORION_XMLfile>\n");
	fprintf(sstfile, "\t\t<core_temperature>350</core_temperature>\n");
	fprintf(sstfile, "\t\t<core_tech_node>65</core_tech_node>\n");
	fprintf(sstfile, "\t\t<frequency>1ns</frequency>\n");
	fprintf(sstfile, "\t\t<power_monitor>YES</power_monitor>\n");
	fprintf(sstfile, "\t\t<temperature_monitor>NO</temperature_monitor>\n");
	fprintf(sstfile, "\t\t<router_voltage>1.1</router_voltage>\n");
	fprintf(sstfile, "\t\t<router_clock_rate>1000000000</router_clock_rate>\n");
	fprintf(sstfile, "\t\t<router_flit_bits>64</router_flit_bits>\n");
	fprintf(sstfile, "\t\t<router_virtual_channel_per_port>2</router_virtual_channel_per_port>\n");
	fprintf(sstfile, "\t\t<router_input_ports>%d</router_input_ports>\n", num_ports);
	fprintf(sstfile, "\t\t<router_output_ports>%d</router_output_ports>\n", num_ports);
	fprintf(sstfile, "\t\t<router_link_length>16691</router_link_length>\n");
	fprintf(sstfile, "\t\t<router_power_model>ORION</router_power_model>\n");
	fprintf(sstfile, "\t\t<router_has_global_link>1</router_has_global_link>\n");
	fprintf(sstfile, "\t\t<router_input_buffer_entries_per_vc>16</router_input_buffer_entries_per_vc>\n");
	fprintf(sstfile, "\t\t<router_link_throughput>1</router_link_throughput>\n");
	fprintf(sstfile, "\t\t<router_link_latency>1</router_link_latency>\n");
	fprintf(sstfile, "\t\t<router_horizontal_nodes>1</router_horizontal_nodes>\n");
	fprintf(sstfile, "\t\t<router_vertical_nodes>1</router_vertical_nodes>\n");
	fprintf(sstfile, "\t\t<router_topology>RING</router_topology>\n");
	fprintf(sstfile, "\t\t<core_number_of_NoCs>%d</core_number_of_NoCs>\n", num_cores);
	fprintf(sstfile, "\t\t<push_introspector>routerIntrospector</push_introspector>\n");

    } else   {
	/* error */
    }

}  /* end of sst_router_param_start() */



void
sst_router_param_end(FILE *sstfile, char *Rname)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t</%s>\n", Rname);
    fprintf(sstfile, "\n");

}  /* end of sst_router_param_end() */



void
sst_body_start(FILE *sstfile)
{

    if (sstfile != NULL)   {
	fprintf(sstfile, "<sst>\n");
    }

}  /* end of sst_body_start() */



void
sst_body_end(FILE *sstfile)
{

    if (sstfile != NULL)   {
	fprintf(sstfile, "</sst>\n");
    }

}  /* end of sst_body_end() */



void
sst_pwr_component(FILE *sstfile, pwr_method_t power_method)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    if (power_method == pwrNone)   {
	/* Nothing to do */
    } else if ((power_method == pwrMcPAT) || (power_method == pwrORION))   {
	fprintf(sstfile, "\t\t<introspector name=\"routerIntrospector\">\n");
	fprintf(sstfile, "\t\t\t<introspector_router>\n");
	fprintf(sstfile, "\t\t\t\t<params>\n");
	fprintf(sstfile, "\t\t\t\t\t<params include=intro1_params />\n");
	fprintf(sstfile, "\t\t\t\t</params>\n");
	fprintf(sstfile, "\t\t\t</introspector_router>\n");
	fprintf(sstfile, "\t\t</introspector>\n");
	fprintf(sstfile, "\n");
    }

}  /* end of sst_pwr_component() */



void
sst_gen_component(char *id, char *net_link_id, char *net_aggregator_id,
	char *nvram_aggregator_id, char *ss_aggregator_id,
	int rank, char *pattern_name, FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t<component name=\"%s\" type=\"%s\">\n", id, pattern_name);
    fprintf(sstfile, "\t\t<params include=Gp>\n");
    fprintf(sstfile, "\t\t\t<rank> %d </rank>\n", rank);
    fprintf(sstfile, "\t\t</params>\n");

    if (net_link_id)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"NoC\" latency=$lat_local_net/>\n",
	    net_link_id);
    }
    if (net_aggregator_id)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"NETWORK\" latency=$lat_global_net/>\n",
	    net_aggregator_id);
    }
    if (nvram_aggregator_id)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"NVRAM\" latency=$lat_local_nvram/>\n",
	    nvram_aggregator_id);
    }
    if (ss_aggregator_id)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"STORAGE\" latency=$lat_storage_net/>\n",
	    ss_aggregator_id);
    }
    fprintf(sstfile, "\t</component>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_gen_component() */



void
sst_nvram_component(char *id, char *link_id, nvram_type_t type, FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t<component name=\"%s\" type=\"bit_bucket\">\n", id);
    if (type == LOCAL_NVRAM)   {
	fprintf(sstfile, "\t\t<params include=NVRAMparams></params>\n");
    }
    if (type == SSD)   {
	fprintf(sstfile, "\t\t<params include=SSDparams></params>\n");
    }

    if (type == LOCAL_NVRAM)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"STORAGE\" latency=$lat_storage_nvram/>\n",
	    link_id);
    }
    if (type == SSD)   {
	fprintf(sstfile, "\t\t<link name=\"%s\" port=\"STORAGE\" latency=$lat_ssd_io/>\n",
	    link_id);
    }
    fprintf(sstfile, "\t</component>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_nvram_component() */



void
sst_router_component_start(char *id, char *cname, router_function_t role,
	pwr_method_t power_method, FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t<component name=\"%s\" ", id);
    if (power_method == pwrNone)   {
	fprintf(sstfile, "type=\"routermodel\">\n");
    } else   {
	fprintf(sstfile, "type=\"routermodel_power\">\n");
    }
    switch (role)   {
	case Rnet:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_NETWORK);
	    break;
	case RNoC:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_NoC);
	    break;
	case RnetPort:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_NET_ACCESS);
	    break;
	case Rnvram:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_NVRAM);
	    break;
	case Rstorage:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_STORAGE);
	    break;
	case RstoreIO:
	    fprintf(sstfile, "\t\t<params include=%s>\n", RNAME_IO);
	    break;
    }
    fprintf(sstfile, "\t\t\t<component_name> %s </component_name>\n", cname);

}  /* end of sst_router_component_start() */



void
sst_router_component_link(char *id, uint64_t link_lat, char *link_name, FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t\t<link name=\"%s\" port=\"%s\" latency=%luns/>\n",
	id, link_name, link_lat);

}  /* end of sst_router_component_link() */



void
sst_router_component_end(FILE *sstfile)
{

    if (sstfile == NULL)   {
	/* Nothing to output */
	return;
    }

    fprintf(sstfile, "\t</component>\n");
    fprintf(sstfile, "\n");

}  /* end of sst_router_component_end() */



void
sst_footer(FILE *sstfile)
{
    /* Nothing to be done here anymore */
}  /* end of sst_footer() */



/*
** Generate the pattern generator components
*/
void
sst_pattern_generators(char *pattern_name, FILE *sstfile)
{

int n, r, p;
int aggregator, aggregator_port;
int nvram, nvram_port;
int ss, ss_port;
char id[MAX_ID_LEN];
char net_link_id[MAX_ID_LEN];
char net_aggregator_id[MAX_ID_LEN];
char nvram_link_id[MAX_ID_LEN];
char nvram_aggregator_id[MAX_ID_LEN];
char ss_link_id[MAX_ID_LEN];
char ss_aggregator_id[MAX_ID_LEN];
char *label;


    if (sstfile == NULL)   {
	return;
    }

    reset_nic_list();
    while (next_nic(&n, &r, &p,
	    &aggregator, &aggregator_port,
	    &nvram, &nvram_port,
	    &ss, &ss_port,
	    &label))   {
	snprintf(id, MAX_ID_LEN, "G%d", n);

	snprintf(net_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	snprintf(net_aggregator_id, MAX_ID_LEN, "R%dP%d", aggregator, aggregator_port);

	snprintf(nvram_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	snprintf(nvram_aggregator_id, MAX_ID_LEN, "R%dP%d", nvram, nvram_port);

	snprintf(ss_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	snprintf(ss_aggregator_id, MAX_ID_LEN, "R%dP%d", ss, ss_port);

	if (r >= 0)   {
	    if (aggregator >= 0)   {
		sst_gen_component(id, net_link_id, net_aggregator_id, nvram_aggregator_id, ss_aggregator_id, n, pattern_name, sstfile);
	    } else   {
		/* No network aggregator in this configuration */
		sst_gen_component(id, net_link_id, NULL, nvram_aggregator_id, ss_aggregator_id, n, pattern_name, sstfile);
	    }
	} else   {
	    /* Single node, no network */
	    if (aggregator >= 0)   {
		sst_gen_component(id, NULL, net_aggregator_id, nvram_aggregator_id, ss_aggregator_id, n, pattern_name, sstfile);
	    } else   {
		/* No network aggregator nor NoC in this configuration */
		sst_gen_component(id, NULL, NULL, nvram_aggregator_id, ss_aggregator_id, n, pattern_name, sstfile);
	    }
	}
    }

}  /* end of sst_pattern_generators() */



/*
** Generate the NVRAM components
*/
void
sst_nvram(FILE *sstfile)
{

int n, r, p;
nvram_type_t t;
char id[MAX_ID_LEN];
char link_id[MAX_ID_LEN];


    if (sstfile == NULL)   {
	return;
    }

    reset_nvram_list();
    while (next_nvram(&n, &r, &p, &t))   {
	if (t == LOCAL_NVRAM)   {
	    snprintf(id, MAX_ID_LEN, "LocalNVRAM%d", n);
	}
	if (t == SSD)   {
	    snprintf(id, MAX_ID_LEN, "SSD%d", n);
	}
	snprintf(link_id, MAX_ID_LEN, "R%dP%d", r, p);

	if (r >= 0)   {
	    sst_nvram_component(id, link_id, t, sstfile);
	}
    }

}  /* end of sst_nvram() */



/*
** Generate the router components
*/
void
sst_routers(FILE *sstfile, uint64_t node_latency, uint64_t net_latency,
	uint64_t nvram_latency, pwr_method_t power_method)
{

int l, r, p;
int lp, rp;
static int flip= 0;
char net_link_id[MAX_ID_LEN];
char nvram_link_id[MAX_ID_LEN];
char router_id[MAX_ID_LEN];
char cname[MAX_ID_LEN];
router_function_t role;
int wormhole;


    if (sstfile == NULL)   {
	return;
    }

    reset_router_list();
    while (next_router(&r, &role, &wormhole))   {
	snprintf(router_id, MAX_ID_LEN, "R%d", r);
	snprintf(cname, MAX_ID_LEN, "R%d", r);
	sst_router_component_start(router_id, cname, role, power_method, sstfile);
	/*
	** We have to list the links in order in the params section, so the router
	** componentn can get the names and create the appropriate links.
	*/

	/* Links to local NIC(s) */
	reset_router_nics(r);
	while (next_router_nic(r, &p))   {
	    snprintf(net_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	    snprintf(cname, MAX_ID_LEN, "Link%dname", p);
	    fprintf(sstfile, "\t\t\t<%s> %s </%s>\n", cname, net_link_id, cname);
	}

	/* Links to other routers */
	reset_router_links(r);
	while (next_router_link(r, &l, &lp, &rp))   {
	    if (l < 0)   {
		snprintf(net_link_id, MAX_ID_LEN, "unused");
		/*
		** This is a bit ugly. We're dealing with a loop back link on a router.
		** It doesn't belong in the routermodel link section, but we have to list
		** it in the parameter section so that the port numbering is sequential.
		** So, once we use the left port, then the right port number.
		*/
		if (flip)   {
		    p= lp;
		    flip= 0;
		} else   {
		    p= rp;
		    flip= 1;
		}
	    } else   {
		p= rp;
		snprintf(net_link_id, MAX_ID_LEN, "L%d", l);
	    }
	    snprintf(cname, MAX_ID_LEN, "Link%dname", p);
	    fprintf(sstfile, "\t\t\t<%s> %s </%s>\n", cname, net_link_id, cname);
	}

	/* Links to NVRAM(s) */
	reset_router_nvram(r);
	while (next_router_nvram(r, &p))   {
	    snprintf(nvram_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	    snprintf(cname, MAX_ID_LEN, "Link%dname", p);
	    fprintf(sstfile, "\t\t\t<%s> %s </%s>\n", cname, nvram_link_id, cname);
	}

	
	if ((power_method == pwrMcPAT) || (power_method == pwrORION))   {
	    fprintf(sstfile, "\t\t\t<router_floorplan_id> %d </router_floorplan_id>\n", r);
	}

	fprintf(sstfile, "\t\t</params>\n");



	/* Now do it again for the links section */

	/* Links to local NIC(s) */
	reset_router_nics(r);
	while (next_router_nic(r, &p))   {
	    snprintf(net_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	    sst_router_component_link(net_link_id, node_latency, net_link_id, sstfile);
	}

	/* Links to other routers */
	reset_router_links(r);
	while (next_router_link(r, &l, &lp, &rp))   {
	    if (l >= 0)   {
		snprintf(net_link_id, MAX_ID_LEN, "L%d", l);
		sst_router_component_link(net_link_id, net_latency, net_link_id, sstfile);
	    } else   {
		/* Don't creat loops back to the same router */
	    }
	}

	/* Link to local NVRAMS(s) */
	reset_router_nvram(r);
	while (next_router_nvram(r, &p))   {
	    snprintf(nvram_link_id, MAX_ID_LEN, "R%dP%d", r, p);
	    sst_router_component_link(nvram_link_id, nvram_latency, nvram_link_id, sstfile);
	}

	sst_router_component_end(sstfile);
    }

}  /* end of sst_routers() */
