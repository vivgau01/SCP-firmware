/*
 * Arm SCP/MCP Software
 * Copyright (c) 2019-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "juno_scmi.h"

#include <mod_scmi.h>
#include <mod_transport.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_macros.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>

static const struct fwk_element element_table[] = {
    [JUNO_SCMI_SERVICE_IDX_PSCI_A2P] = {
        .name = "PSCI",
        .data = &(struct mod_scmi_service_config) {
            .transport_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                JUNO_SCMI_SERVICE_IDX_PSCI_A2P),
            .transport_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                MOD_TRANSPORT_API_IDX_SCMI_TO_TRANSPORT),
            .transport_notification_init_id =
                FWK_ID_NOTIFICATION_INIT(FWK_MODULE_IDX_TRANSPORT,
                    MOD_TRANSPORT_NOTIFICATION_IDX_INITIALIZED),
            .scmi_agent_id = (unsigned int) JUNO_SCMI_AGENT_IDX_PSCI,
            .scmi_p2a_id = FWK_ID_NONE_INIT,
        },
    },

    [JUNO_SCMI_SERVICE_IDX_OSPM_A2P_0] = {
        .name = "OSPM-A2P-0",
        .data = &(struct mod_scmi_service_config) {
            .transport_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                JUNO_SCMI_SERVICE_IDX_OSPM_A2P_0),
            .transport_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                MOD_TRANSPORT_API_IDX_SCMI_TO_TRANSPORT),
            .transport_notification_init_id =
                FWK_ID_NOTIFICATION_INIT(FWK_MODULE_IDX_TRANSPORT,
                    MOD_TRANSPORT_NOTIFICATION_IDX_INITIALIZED),
            .scmi_agent_id = (unsigned int) JUNO_SCMI_AGENT_IDX_OSPM,
#ifdef BUILD_HAS_SCMI_NOTIFICATIONS
            .scmi_p2a_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_SCMI,
                JUNO_SCMI_SERVICE_IDX_OSPM_P2A),
#else
            .scmi_p2a_id = FWK_ID_NONE_INIT,
#endif
        },
    },

    [JUNO_SCMI_SERVICE_IDX_OSPM_A2P_1] = {
        .name = "OSPM-A2P-1",
        .data = &(struct mod_scmi_service_config) {
            .transport_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                JUNO_SCMI_SERVICE_IDX_OSPM_A2P_1),
            .transport_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                MOD_TRANSPORT_API_IDX_SCMI_TO_TRANSPORT),
            .transport_notification_init_id =
                FWK_ID_NOTIFICATION_INIT(FWK_MODULE_IDX_TRANSPORT,
                    MOD_TRANSPORT_NOTIFICATION_IDX_INITIALIZED),
            .scmi_agent_id = (unsigned int) JUNO_SCMI_AGENT_IDX_OSPM,
#ifdef BUILD_HAS_SCMI_NOTIFICATIONS
            .scmi_p2a_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_SCMI,
                JUNO_SCMI_SERVICE_IDX_OSPM_P2A),
#else
            .scmi_p2a_id = FWK_ID_NONE_INIT,
#endif
        },
    },

#ifdef BUILD_HAS_SCMI_NOTIFICATIONS
    [JUNO_SCMI_SERVICE_IDX_OSPM_P2A] = {
        .name = "OSPM-P2A",
        .data = &(struct mod_scmi_service_config) {
            .transport_id = FWK_ID_ELEMENT_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                JUNO_SCMI_SERVICE_IDX_OSPM_P2A),
            .transport_api_id = FWK_ID_API_INIT(
                FWK_MODULE_IDX_TRANSPORT,
                MOD_TRANSPORT_API_IDX_SCMI_TO_TRANSPORT),
            .transport_notification_init_id =
                FWK_ID_NOTIFICATION_INIT(FWK_MODULE_IDX_TRANSPORT,
                    MOD_TRANSPORT_NOTIFICATION_IDX_INITIALIZED),
            .scmi_agent_id = (unsigned int) JUNO_SCMI_AGENT_IDX_OSPM,
            .scmi_p2a_id = FWK_ID_NONE_INIT,
        },
    },
#endif

    [JUNO_SCMI_SERVICE_IDX_COUNT] = { 0 },
};

static const struct fwk_element *get_element_table(fwk_id_t module_id)
{
    return element_table;
}

#ifndef BUILD_HAS_MOD_RESOURCE_PERMS

/* PSCI agent has no access to clock, perf and sensor protocol
 */
static const uint32_t dis_protocol_list_psci[] = {
    MOD_SCMI_PROTOCOL_ID_SENSOR,
    MOD_SCMI_PROTOCOL_ID_CLOCK,
    MOD_SCMI_PROTOCOL_ID_PERF,
};
#endif

static const struct mod_scmi_agent agent_table[] = {
    [JUNO_SCMI_AGENT_IDX_OSPM] = {
        .type = SCMI_AGENT_TYPE_OSPM,
        .name = "OSPM",
    },
    [JUNO_SCMI_AGENT_IDX_PSCI] = {
        .type = SCMI_AGENT_TYPE_PSCI,
        .name = "PSCI",
    },
};

struct fwk_module_config config_scmi = {
    .data =
        &(struct mod_scmi_config){
            .protocol_count_max = 6,
#ifndef BUILD_HAS_MOD_RESOURCE_PERMS
            .dis_protocol_count_psci = FWK_ARRAY_SIZE(dis_protocol_list_psci),
            .dis_protocol_list_psci = dis_protocol_list_psci,
#endif
            .agent_count = FWK_ARRAY_SIZE(agent_table) - 1,
            .agent_table = agent_table,
            .vendor_identifier = "arm",
            .sub_vendor_identifier = "arm",
        },

    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_element_table),
};
