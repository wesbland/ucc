/**
 * Copyright (C) Mellanox Technologies Ltd. 2021.  ALL RIGHTS RESERVED.
 *
 * See file LICENSE for terms.
 */
#include "config.h"

#ifndef UCC_TL_UCP_EP_H_
#define UCC_TL_UCP_EP_H_
#include "ucc/api/ucc.h"
#include <ucp/api/ucp.h>
#include "tl_ucp.h"
#include "tl_ucp_addr.h"
typedef struct ucc_tl_ucp_context ucc_tl_ucp_context_t;
typedef struct ucc_tl_ucp_team    ucc_tl_ucp_team_t;


ucc_status_t ucc_tl_ucp_connect_team_ep(ucc_tl_ucp_team_t *team, ucc_rank_t team_rank,
                                        ucc_context_id_t key, ucp_ep_h *ep);
ucc_status_t ucc_tl_ucp_connect_ctx_ep(ucc_tl_ucp_context_t *ctx, ucc_rank_t ctx_rank);

ucc_status_t ucc_tl_ucp_close_eps(ucc_tl_ucp_context_t *ctx);

static inline ucc_context_id_t
ucc_tl_ucp_get_rank_key(ucc_tl_ucp_team_t *team, ucc_rank_t rank)
{
    size_t max_addrlen = team->addr_storage->max_addrlen;
    char *addresses = (char*)team->addr_storage->addresses;
    ucc_tl_ucp_addr_t  *address = (ucc_tl_ucp_addr_t *)(addresses +
                                                        max_addrlen * rank);
    return address->id;
}

static inline ucc_status_t ucc_tl_ucp_get_ep(ucc_tl_ucp_team_t *team, ucc_rank_t rank,
                                             ucp_ep_h *ep)
{
    ucc_tl_ucp_context_t *ctx = UCC_TL_UCP_TEAM_CTX(team);
    ucc_status_t          status;
    if (!ctx->eps) {
        ucc_context_id_t key = ucc_tl_ucp_get_rank_key(team, rank);
        *ep = tl_ucp_hash_get(ctx->ep_hash, key);
        if (NULL == (*ep)) {
            /* Not connected yet */
            status = ucc_tl_ucp_connect_team_ep(team, rank, key, ep);
            if (UCC_OK != status) {
                tl_error(UCC_TL_TEAM_LIB(team), "failed to connect team ep");
                *ep = NULL;
                return status;
            }
        }
    } else {
        ucc_assert(ctx->eps);
        ucc_rank_t ctx_rank = -1; //TODO map to ctx rank
        ucc_assert(0);
        if (NULL == ctx->eps[ctx_rank]) {
            /* Not connected yet */
            status = ucc_tl_ucp_connect_ctx_ep(ctx, ctx_rank);
            if (UCC_OK != status) {
                tl_error(UCC_TL_TEAM_LIB(team), "failed to connect ctx ep");
                *ep = NULL;
                return status;
            }
        }
        ucc_assert(ctx->eps[ctx_rank]);
        *ep = ctx->eps[ctx_rank];
    }
    return UCC_OK;
}

#endif
