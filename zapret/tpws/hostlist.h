#pragma once

#include <stdbool.h>
#include "pools.h"
#include "params.h"

bool AppendHostlistItem(strpool **hostlist, char *s);
bool AppendHostList(strpool **hostlist, const char *filename);
bool LoadAllHostLists();
bool NonEmptyHostlist(strpool **hostlist);
// return : true = apply fooling, false = do not apply
bool HostlistCheck(const struct desync_profile *dp,const char *host, bool *excluded, bool bSkipReloadCheck);
struct hostlist_file *RegisterHostlist(struct desync_profile *dp, bool bExclude, const char *filename);
bool HostlistsReloadCheckForProfile(const struct desync_profile *dp);
void HostlistsDebug();
