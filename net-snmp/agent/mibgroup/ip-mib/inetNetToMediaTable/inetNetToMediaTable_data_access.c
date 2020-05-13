/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.17 $ of : mfd-data-access.m2c,v $ 
 *
 * $Id$
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "inetNetToMediaTable.h"


#include "inetNetToMediaTable_data_access.h"

netsnmp_feature_require(container_lifo);
static netsnmp_arp_access * arp_access = NULL;

/** @ingroup interface 
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table inetNetToMediaTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * IP-MIB::inetNetToMediaTable is subid 35 of ip.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.4.35, length: 8
 */

/**
 * initialization for inetNetToMediaTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param inetNetToMediaTable_reg
 *        Pointer to inetNetToMediaTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
inetNetToMediaTable_init_data(inetNetToMediaTable_registration *
                              inetNetToMediaTable_reg)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_init_data", "called\n"));

    /*
     * TODO:303:o: Initialize inetNetToMediaTable data.
     */

    return MFD_SUCCESS;
}                               /* inetNetToMediaTable_init_data */

/**
 * container overview
 *
 */

/**
 * check entry for update
 */
static void
_add_or_update_arp_entry(netsnmp_arp_entry *arp_entry,
                 netsnmp_container *container)
{
    inetNetToMediaTable_rowreq_ctx *rowreq_ctx, *old;
    int             inetAddressType;

    DEBUGTRACE;

    netsnmp_assert(NULL != arp_entry);
    netsnmp_assert(NULL != container);

    /*
     * convert the addr len to an inetAddressType
     */
    switch (arp_entry->arp_ipaddress_len) {
    case 4:
        inetAddressType = INETADDRESSTYPE_IPV4;
        break;

    case 16:
        inetAddressType = INETADDRESSTYPE_IPV6;
        break;

    default:
        snmp_log(LOG_ERR, "inetNetToMediaTable:_add_or_update_arpentry: unsupported address type, len = %d\n", arp_entry->arp_ipaddress_len);
        netsnmp_access_arp_entry_free(arp_entry);
        return;
    }

    /*
     * allocate an row context and set the index(es), then try to find it in
     * the cache.
     */
    rowreq_ctx = inetNetToMediaTable_allocate_rowreq_ctx(arp_entry, NULL);
    if ((NULL != rowreq_ctx) &&
        (MFD_SUCCESS == inetNetToMediaTable_indexes_set
         (rowreq_ctx, rowreq_ctx->data->if_index, inetAddressType,
          (char *) rowreq_ctx->data->arp_ipaddress,
          rowreq_ctx->data->arp_ipaddress_len))) {

        /* try to find old entry */
        old = (inetNetToMediaTable_rowreq_ctx*)CONTAINER_FIND(container, rowreq_ctx);
        if (arp_entry->flags & NETSNMP_ACCESS_ARP_ENTRY_FLAG_DELETE) {
            /* delete existing entry */
            if (old != NULL) {
                CONTAINER_REMOVE(container, old);
                inetNetToMediaTable_release_rowreq_ctx(old);
            }
            inetNetToMediaTable_release_rowreq_ctx(rowreq_ctx);
        } else if (old != NULL) {
            /* the entry is already there, update it */
            netsnmp_access_arp_entry_update(old->data, arp_entry);
            /* delete the auxiliary context we used to find the entry
             * (this deletes also arp_entry) */
            inetNetToMediaTable_release_rowreq_ctx(rowreq_ctx);
        } else {
            /* create new entry and add it to the cache*/
            rowreq_ctx->inetNetToMediaRowStatus = ROWSTATUS_ACTIVE;
            rowreq_ctx->data->arp_last_updated = netsnmp_get_agent_uptime();
            CONTAINER_INSERT(container, rowreq_ctx);
        }
    } else {
        if (rowreq_ctx) {
            snmp_log(LOG_ERR, "error setting index while loading "
                     "inetNetToMediaTable cache.\n");
            inetNetToMediaTable_release_rowreq_ctx(rowreq_ctx);
        } else
            netsnmp_access_arp_entry_free(arp_entry);
    }
}

static void  _arp_hook_update(netsnmp_arp_access *access, netsnmp_arp_entry *entry)
{
    _add_or_update_arp_entry(entry, access->magic);
}

typedef struct {
    unsigned generation;
    netsnmp_container *to_delete;
} _collect_ctx;

/**
 * Put all entries with outdated generation to deletion list.
 */
static void
_collect_invalid_arp_ctx(inetNetToMediaTable_rowreq_ctx *ctx,
                         _collect_ctx *cctx)
{
    if (ctx->data->generation != cctx->generation)
        CONTAINER_INSERT(cctx->to_delete, ctx);
}

static void _arp_hook_gc(netsnmp_arp_access *access)
{
    netsnmp_container *container = access->magic;
    _collect_ctx cctx;

    cctx.to_delete = netsnmp_container_find("lifo");
    cctx.generation = access->generation;

    CONTAINER_FOR_EACH(container,
                       (netsnmp_container_obj_func *) _collect_invalid_arp_ctx,
                       &cctx);

    while (CONTAINER_SIZE(cctx.to_delete)) {
        inetNetToMediaTable_rowreq_ctx *ctx = (inetNetToMediaTable_rowreq_ctx*)CONTAINER_FIRST(cctx.to_delete);
        CONTAINER_REMOVE(container, ctx);
        inetNetToMediaTable_release_rowreq_ctx(ctx);
        CONTAINER_REMOVE(cctx.to_delete, NULL);
    }
    CONTAINER_FREE(cctx.to_delete);
}

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 * @param  cache A pointer to a cache structure. You can set the timeout
 *         and other cache flags using this pointer.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 *  This is also the place to set up cache behavior. The default, to
 *  simply set the cache timeout, will work well with the default
 *  container. If you are using a custom container, you may want to
 *  look at the cache helper documentation to see if there are any
 *  flags you want to set.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
inetNetToMediaTable_container_init(netsnmp_container **container_ptr_ptr,
                                   netsnmp_cache * cache)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_container_init", "called\n"));

    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,
                 "bad container param to inetNetToMediaTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,
                 "bad cache param to inetNetToMediaTable_container_init\n");
        return;
    }

    arp_access = netsnmp_access_arp_create(
                           NETSNMP_ACCESS_ARP_CREATE_NOFLAGS,
                           _arp_hook_update,
                           _arp_hook_gc,
                           &cache->timeout,
                           &cache->flags,
                           &cache->expired);
    if (arp_access == NULL) {
        snmp_log(LOG_ERR,
                 "unable to create arp access in inetNetToMediaTable_container_init\n");
        return;
    }
}                               /* inetNetToMediaTable_container_init */

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before inetNetToMediaTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
inetNetToMediaTable_container_shutdown(netsnmp_container *container_ptr)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_container_shutdown", "called\n"));

    if (NULL != arp_access) {
        netsnmp_access_arp_delete(arp_access);
        arp_access = NULL;
    }

    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,
                 "bad params to inetNetToMediaTable_container_shutdown\n");
        return;
    }
}                               /* inetNetToMediaTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement inetNetToMediaTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  inetNetToMediaTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
inetNetToMediaTable_container_load(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_cache_load", "called\n"));

    arp_access->magic = container;
    if (netsnmp_access_arp_load(arp_access) < 0)
        return MFD_ERROR;

    return MFD_SUCCESS;
}                               /* inetNetToMediaTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
inetNetToMediaTable_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_container_free", "called\n"));

    if (NULL != arp_access) {
        netsnmp_access_arp_unload(arp_access);
        arp_access->magic = NULL;
    }

    /*
     * TODO:380:M: Free inetNetToMediaTable container data.
     */
}                               /* inetNetToMediaTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
inetNetToMediaTable_row_prep(inetNetToMediaTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_row_prep",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
}                               /* inetNetToMediaTable_row_prep */

/*
 * TODO:420:r: Implement inetNetToMediaTable index validation.
 */
/*---------------------------------------------------------------------
 * IP-MIB::inetNetToMediaEntry.inetNetToMediaIfIndex
 * inetNetToMediaIfIndex is subid 1 of inetNetToMediaEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.35.1.1
 * Description:
The index value which uniquely identifies the interface to
            which this entry is applicable.  The interface identified by
            a particular value of this index is the same interface as
            identified by the same value of the IF-MIB's ifIndex.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: d
 *
 * Ranges:  1 - 2147483647;
 *
 * Its syntax is InterfaceIndex (based on perltype INTEGER32)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (long)
 *
 *
 *
 * NOTE: NODE inetNetToMediaIfIndex IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetNetToMediaIfIndex index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetNetToMediaTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The value is in (one of) the range set(s):  1 - 2147483647
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetNetToMediaIfIndex_check_index(inetNetToMediaTable_rowreq_ctx *
                                  rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaIfIndex_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetNetToMediaTable index inetNetToMediaIfIndex.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetNetToMediaIfIndex)
     */

    return MFD_SUCCESS;         /* inetNetToMediaIfIndex index ok */
}                               /* inetNetToMediaIfIndex_check_index */

/*---------------------------------------------------------------------
 * IP-MIB::inetNetToMediaEntry.inetNetToMediaNetAddressType
 * inetNetToMediaNetAddressType is subid 2 of inetNetToMediaEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.35.1.2
 * Description:
The type of inetNetToMediaNetAddress.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  1      hasdefval 0
 *   readable   0     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 * Enum range: 5/8. Values:  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * Its syntax is InetAddressType (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 *
 *
 *
 * NOTE: NODE inetNetToMediaNetAddressType IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetNetToMediaNetAddressType index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetNetToMediaTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The value is one of  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetNetToMediaNetAddressType_check_index(inetNetToMediaTable_rowreq_ctx *
                                         rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaNetAddressType_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetNetToMediaTable index inetNetToMediaNetAddressType.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetNetToMediaNetAddressType)
     */

    return MFD_SUCCESS;         /* inetNetToMediaNetAddressType index ok */
}                               /* inetNetToMediaNetAddressType_check_index */

/*---------------------------------------------------------------------
 * IP-MIB::inetNetToMediaEntry.inetNetToMediaNetAddress
 * inetNetToMediaNetAddress is subid 3 of inetNetToMediaEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.35.1.3
 * Description:
The IP Address corresponding to the media-dependent
            `physical' address.  The address type of this object is
            specified in inetNetToMediaAddressType.


            Implementors need to be aware that if the size of
            inetNetToMediaNetAddress exceeds 115 octets then OIDS of
            instances of columns in this row will have more than 128
            sub-identifiers and cannot be accessed using SNMPv1, SNMPv2c
            or SNMPv3.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is InetAddress (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE inetNetToMediaNetAddress IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetNetToMediaNetAddress index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetNetToMediaTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The length is in (one of) the range set(s):  0 - 255
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetNetToMediaNetAddress_check_index(inetNetToMediaTable_rowreq_ctx *
                                     rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaNetAddress_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetNetToMediaTable index inetNetToMediaNetAddress.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetNetToMediaNetAddress)
     */

    return MFD_SUCCESS;         /* inetNetToMediaNetAddress index ok */
}                               /* inetNetToMediaNetAddress_check_index */

/**
 * verify specified index is valid.
 *
 * This check is independent of whether or not the values specified for
 * the columns of the new row are valid. Column values and row consistency
 * will be checked later. At this point, only the index values should be
 * checked.
 *
 * All of the individual index validation functions have been called, so this
 * is the place to make sure they are valid as a whole when combined. If
 * you only have one index, then you probably don't need to do anything else
 * here.
 * 
 * @note Keep in mind that if the indexes refer to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 *
 * @param inetNetToMediaTable_reg
 *        Pointer to the user registration data
 * @param rowreq_ctx
 *        Pointer to the users context.
 * @retval MFD_SUCCESS            : success
 * @retval MFD_CANNOT_CREATE_NOW  : index not valid right now
 * @retval MFD_CANNOT_CREATE_EVER : index never valid
 */
int
inetNetToMediaTable_validate_index(inetNetToMediaTable_registration *
                                   inetNetToMediaTable_reg,
                                   inetNetToMediaTable_rowreq_ctx *
                                   rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:inetNetToMediaTable:inetNetToMediaTable_validate_index", "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:430:M: |-> Validate potential inetNetToMediaTable index.
     */
    if (1) {
        snmp_log(LOG_WARNING, "invalid index for a new row in the "
                 "inetNetToMediaTable table.\n");
        /*
         * determine failure type.
         *
         * If the index could not ever be created, return MFD_NOT_EVER
         * If the index can not be created under the present circumstances
         * (even though it could be created under other circumstances),
         * return MFD_NOT_NOW.
         */
        if (0) {
            return MFD_CANNOT_CREATE_EVER;
        } else {
            return MFD_CANNOT_CREATE_NOW;
        }
    }

    return rc;
}                               /* inetNetToMediaTable_validate_index */

/** @} */
