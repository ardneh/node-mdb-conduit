// AUTO-GENERATED FILE DO NOT EDIT
// See src/mongo/db/auth/generate_action_types.py
/*    Copyright 2012 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "mongo/pch.h"

#include "mongo/db/auth/action_type.h"

#include <iostream>
#include <string>

#include "mongo/base/status.h"
#include "mongo/platform/cstdint.h"
#include "mongo/util/mongoutils/str.h"

namespace mongo {

    const ActionType ActionType::addShard(addShardValue);
    const ActionType ActionType::anyAction(anyActionValue);
    const ActionType ActionType::appendOplogNote(appendOplogNoteValue);
    const ActionType ActionType::applicationMessage(applicationMessageValue);
    const ActionType ActionType::auditLogRotate(auditLogRotateValue);
    const ActionType ActionType::authCheck(authCheckValue);
    const ActionType ActionType::authSchemaUpgrade(authSchemaUpgradeValue);
    const ActionType ActionType::authenticate(authenticateValue);
    const ActionType ActionType::changeCustomData(changeCustomDataValue);
    const ActionType ActionType::changePassword(changePasswordValue);
    const ActionType ActionType::changeOwnPassword(changeOwnPasswordValue);
    const ActionType ActionType::changeOwnCustomData(changeOwnCustomDataValue);
    const ActionType ActionType::cleanupOrphaned(cleanupOrphanedValue);
    const ActionType ActionType::closeAllDatabases(closeAllDatabasesValue);
    const ActionType ActionType::collMod(collModValue);
    const ActionType ActionType::collStats(collStatsValue);
    const ActionType ActionType::compact(compactValue);
    const ActionType ActionType::connPoolStats(connPoolStatsValue);
    const ActionType ActionType::connPoolSync(connPoolSyncValue);
    const ActionType ActionType::convertToCapped(convertToCappedValue);
    const ActionType ActionType::cpuProfiler(cpuProfilerValue);
    const ActionType ActionType::createCollection(createCollectionValue);
    const ActionType ActionType::createDatabase(createDatabaseValue);
    const ActionType ActionType::createIndex(createIndexValue);
    const ActionType ActionType::createRole(createRoleValue);
    const ActionType ActionType::createUser(createUserValue);
    const ActionType ActionType::cursorInfo(cursorInfoValue);
    const ActionType ActionType::dbHash(dbHashValue);
    const ActionType ActionType::dbStats(dbStatsValue);
    const ActionType ActionType::diagLogging(diagLoggingValue);
    const ActionType ActionType::dropAllRolesFromDatabase(dropAllRolesFromDatabaseValue);
    const ActionType ActionType::dropAllUsersFromDatabase(dropAllUsersFromDatabaseValue);
    const ActionType ActionType::dropCollection(dropCollectionValue);
    const ActionType ActionType::dropDatabase(dropDatabaseValue);
    const ActionType ActionType::dropIndex(dropIndexValue);
    const ActionType ActionType::dropRole(dropRoleValue);
    const ActionType ActionType::dropUser(dropUserValue);
    const ActionType ActionType::emptycapped(emptycappedValue);
    const ActionType ActionType::enableProfiler(enableProfilerValue);
    const ActionType ActionType::enableSharding(enableShardingValue);
    const ActionType ActionType::find(findValue);
    const ActionType ActionType::flushRouterConfig(flushRouterConfigValue);
    const ActionType ActionType::fsync(fsyncValue);
    const ActionType ActionType::getCmdLineOpts(getCmdLineOptsValue);
    const ActionType ActionType::getLog(getLogValue);
    const ActionType ActionType::getParameter(getParameterValue);
    const ActionType ActionType::getShardMap(getShardMapValue);
    const ActionType ActionType::getShardVersion(getShardVersionValue);
    const ActionType ActionType::grantRole(grantRoleValue);
    const ActionType ActionType::grantPrivilegesToRole(grantPrivilegesToRoleValue);
    const ActionType ActionType::grantRolesToRole(grantRolesToRoleValue);
    const ActionType ActionType::grantRolesToUser(grantRolesToUserValue);
    const ActionType ActionType::hostInfo(hostInfoValue);
    const ActionType ActionType::impersonate(impersonateValue);
    const ActionType ActionType::indexStats(indexStatsValue);
    const ActionType ActionType::inprog(inprogValue);
    const ActionType ActionType::insert(insertValue);
    const ActionType ActionType::internal(internalValue);
    const ActionType ActionType::invalidateUserCache(invalidateUserCacheValue);
    const ActionType ActionType::killCursors(killCursorsValue);
    const ActionType ActionType::killop(killopValue);
    const ActionType ActionType::listDatabases(listDatabasesValue);
    const ActionType ActionType::listShards(listShardsValue);
    const ActionType ActionType::logRotate(logRotateValue);
    const ActionType ActionType::moveChunk(moveChunkValue);
    const ActionType ActionType::netstat(netstatValue);
    const ActionType ActionType::planCacheIndexFilter(planCacheIndexFilterValue);
    const ActionType ActionType::planCacheRead(planCacheReadValue);
    const ActionType ActionType::planCacheWrite(planCacheWriteValue);
    const ActionType ActionType::reIndex(reIndexValue);
    const ActionType ActionType::remove(removeValue);
    const ActionType ActionType::removeShard(removeShardValue);
    const ActionType ActionType::renameCollection(renameCollectionValue);
    const ActionType ActionType::renameCollectionSameDB(renameCollectionSameDBValue);
    const ActionType ActionType::repairDatabase(repairDatabaseValue);
    const ActionType ActionType::replSetConfigure(replSetConfigureValue);
    const ActionType ActionType::replSetGetStatus(replSetGetStatusValue);
    const ActionType ActionType::replSetHeartbeat(replSetHeartbeatValue);
    const ActionType ActionType::replSetReconfig(replSetReconfigValue);
    const ActionType ActionType::replSetStateChange(replSetStateChangeValue);
    const ActionType ActionType::resync(resyncValue);
    const ActionType ActionType::revokeRole(revokeRoleValue);
    const ActionType ActionType::revokePrivilegesFromRole(revokePrivilegesFromRoleValue);
    const ActionType ActionType::revokeRolesFromRole(revokeRolesFromRoleValue);
    const ActionType ActionType::revokeRolesFromUser(revokeRolesFromUserValue);
    const ActionType ActionType::serverStatus(serverStatusValue);
    const ActionType ActionType::setParameter(setParameterValue);
    const ActionType ActionType::shardCollection(shardCollectionValue);
    const ActionType ActionType::shardingState(shardingStateValue);
    const ActionType ActionType::shutdown(shutdownValue);
    const ActionType ActionType::splitChunk(splitChunkValue);
    const ActionType ActionType::splitVector(splitVectorValue);
    const ActionType ActionType::storageDetails(storageDetailsValue);
    const ActionType ActionType::top(topValue);
    const ActionType ActionType::touch(touchValue);
    const ActionType ActionType::unlock(unlockValue);
    const ActionType ActionType::update(updateValue);
    const ActionType ActionType::updateRole(updateRoleValue);
    const ActionType ActionType::updateUser(updateUserValue);
    const ActionType ActionType::validate(validateValue);
    const ActionType ActionType::viewRole(viewRoleValue);
    const ActionType ActionType::viewUser(viewUserValue);

    bool ActionType::operator==(const ActionType& rhs) const {
        return _identifier == rhs._identifier;
    }

    std::ostream& operator<<(std::ostream& os, const ActionType& at) {
        os << ActionType::actionToString(at);
        return os;
    }

    std::string ActionType::toString() const {
        return actionToString(*this);
    }

    Status ActionType::parseActionFromString(const std::string& action, ActionType* result) {
        if (action == "addShard") {
            *result = addShard;
            return Status::OK();
        }
        if (action == "anyAction") {
            *result = anyAction;
            return Status::OK();
        }
        if (action == "appendOplogNote") {
            *result = appendOplogNote;
            return Status::OK();
        }
        if (action == "applicationMessage") {
            *result = applicationMessage;
            return Status::OK();
        }
        if (action == "auditLogRotate") {
            *result = auditLogRotate;
            return Status::OK();
        }
        if (action == "authCheck") {
            *result = authCheck;
            return Status::OK();
        }
        if (action == "authSchemaUpgrade") {
            *result = authSchemaUpgrade;
            return Status::OK();
        }
        if (action == "authenticate") {
            *result = authenticate;
            return Status::OK();
        }
        if (action == "changeCustomData") {
            *result = changeCustomData;
            return Status::OK();
        }
        if (action == "changePassword") {
            *result = changePassword;
            return Status::OK();
        }
        if (action == "changeOwnPassword") {
            *result = changeOwnPassword;
            return Status::OK();
        }
        if (action == "changeOwnCustomData") {
            *result = changeOwnCustomData;
            return Status::OK();
        }
        if (action == "cleanupOrphaned") {
            *result = cleanupOrphaned;
            return Status::OK();
        }
        if (action == "closeAllDatabases") {
            *result = closeAllDatabases;
            return Status::OK();
        }
        if (action == "collMod") {
            *result = collMod;
            return Status::OK();
        }
        if (action == "collStats") {
            *result = collStats;
            return Status::OK();
        }
        if (action == "compact") {
            *result = compact;
            return Status::OK();
        }
        if (action == "connPoolStats") {
            *result = connPoolStats;
            return Status::OK();
        }
        if (action == "connPoolSync") {
            *result = connPoolSync;
            return Status::OK();
        }
        if (action == "convertToCapped") {
            *result = convertToCapped;
            return Status::OK();
        }
        if (action == "cpuProfiler") {
            *result = cpuProfiler;
            return Status::OK();
        }
        if (action == "createCollection") {
            *result = createCollection;
            return Status::OK();
        }
        if (action == "createDatabase") {
            *result = createDatabase;
            return Status::OK();
        }
        if (action == "createIndex") {
            *result = createIndex;
            return Status::OK();
        }
        if (action == "createRole") {
            *result = createRole;
            return Status::OK();
        }
        if (action == "createUser") {
            *result = createUser;
            return Status::OK();
        }
        if (action == "cursorInfo") {
            *result = cursorInfo;
            return Status::OK();
        }
        if (action == "dbHash") {
            *result = dbHash;
            return Status::OK();
        }
        if (action == "dbStats") {
            *result = dbStats;
            return Status::OK();
        }
        if (action == "diagLogging") {
            *result = diagLogging;
            return Status::OK();
        }
        if (action == "dropAllRolesFromDatabase") {
            *result = dropAllRolesFromDatabase;
            return Status::OK();
        }
        if (action == "dropAllUsersFromDatabase") {
            *result = dropAllUsersFromDatabase;
            return Status::OK();
        }
        if (action == "dropCollection") {
            *result = dropCollection;
            return Status::OK();
        }
        if (action == "dropDatabase") {
            *result = dropDatabase;
            return Status::OK();
        }
        if (action == "dropIndex") {
            *result = dropIndex;
            return Status::OK();
        }
        if (action == "dropRole") {
            *result = dropRole;
            return Status::OK();
        }
        if (action == "dropUser") {
            *result = dropUser;
            return Status::OK();
        }
        if (action == "emptycapped") {
            *result = emptycapped;
            return Status::OK();
        }
        if (action == "enableProfiler") {
            *result = enableProfiler;
            return Status::OK();
        }
        if (action == "enableSharding") {
            *result = enableSharding;
            return Status::OK();
        }
        if (action == "find") {
            *result = find;
            return Status::OK();
        }
        if (action == "flushRouterConfig") {
            *result = flushRouterConfig;
            return Status::OK();
        }
        if (action == "fsync") {
            *result = fsync;
            return Status::OK();
        }
        if (action == "getCmdLineOpts") {
            *result = getCmdLineOpts;
            return Status::OK();
        }
        if (action == "getLog") {
            *result = getLog;
            return Status::OK();
        }
        if (action == "getParameter") {
            *result = getParameter;
            return Status::OK();
        }
        if (action == "getShardMap") {
            *result = getShardMap;
            return Status::OK();
        }
        if (action == "getShardVersion") {
            *result = getShardVersion;
            return Status::OK();
        }
        if (action == "grantRole") {
            *result = grantRole;
            return Status::OK();
        }
        if (action == "grantPrivilegesToRole") {
            *result = grantPrivilegesToRole;
            return Status::OK();
        }
        if (action == "grantRolesToRole") {
            *result = grantRolesToRole;
            return Status::OK();
        }
        if (action == "grantRolesToUser") {
            *result = grantRolesToUser;
            return Status::OK();
        }
        if (action == "hostInfo") {
            *result = hostInfo;
            return Status::OK();
        }
        if (action == "impersonate") {
            *result = impersonate;
            return Status::OK();
        }
        if (action == "indexStats") {
            *result = indexStats;
            return Status::OK();
        }
        if (action == "inprog") {
            *result = inprog;
            return Status::OK();
        }
        if (action == "insert") {
            *result = insert;
            return Status::OK();
        }
        if (action == "internal") {
            *result = internal;
            return Status::OK();
        }
        if (action == "invalidateUserCache") {
            *result = invalidateUserCache;
            return Status::OK();
        }
        if (action == "killCursors") {
            *result = killCursors;
            return Status::OK();
        }
        if (action == "killop") {
            *result = killop;
            return Status::OK();
        }
        if (action == "listDatabases") {
            *result = listDatabases;
            return Status::OK();
        }
        if (action == "listShards") {
            *result = listShards;
            return Status::OK();
        }
        if (action == "logRotate") {
            *result = logRotate;
            return Status::OK();
        }
        if (action == "moveChunk") {
            *result = moveChunk;
            return Status::OK();
        }
        if (action == "netstat") {
            *result = netstat;
            return Status::OK();
        }
        if (action == "planCacheIndexFilter") {
            *result = planCacheIndexFilter;
            return Status::OK();
        }
        if (action == "planCacheRead") {
            *result = planCacheRead;
            return Status::OK();
        }
        if (action == "planCacheWrite") {
            *result = planCacheWrite;
            return Status::OK();
        }
        if (action == "reIndex") {
            *result = reIndex;
            return Status::OK();
        }
        if (action == "remove") {
            *result = remove;
            return Status::OK();
        }
        if (action == "removeShard") {
            *result = removeShard;
            return Status::OK();
        }
        if (action == "renameCollection") {
            *result = renameCollection;
            return Status::OK();
        }
        if (action == "renameCollectionSameDB") {
            *result = renameCollectionSameDB;
            return Status::OK();
        }
        if (action == "repairDatabase") {
            *result = repairDatabase;
            return Status::OK();
        }
        if (action == "replSetConfigure") {
            *result = replSetConfigure;
            return Status::OK();
        }
        if (action == "replSetGetStatus") {
            *result = replSetGetStatus;
            return Status::OK();
        }
        if (action == "replSetHeartbeat") {
            *result = replSetHeartbeat;
            return Status::OK();
        }
        if (action == "replSetReconfig") {
            *result = replSetReconfig;
            return Status::OK();
        }
        if (action == "replSetStateChange") {
            *result = replSetStateChange;
            return Status::OK();
        }
        if (action == "resync") {
            *result = resync;
            return Status::OK();
        }
        if (action == "revokeRole") {
            *result = revokeRole;
            return Status::OK();
        }
        if (action == "revokePrivilegesFromRole") {
            *result = revokePrivilegesFromRole;
            return Status::OK();
        }
        if (action == "revokeRolesFromRole") {
            *result = revokeRolesFromRole;
            return Status::OK();
        }
        if (action == "revokeRolesFromUser") {
            *result = revokeRolesFromUser;
            return Status::OK();
        }
        if (action == "serverStatus") {
            *result = serverStatus;
            return Status::OK();
        }
        if (action == "setParameter") {
            *result = setParameter;
            return Status::OK();
        }
        if (action == "shardCollection") {
            *result = shardCollection;
            return Status::OK();
        }
        if (action == "shardingState") {
            *result = shardingState;
            return Status::OK();
        }
        if (action == "shutdown") {
            *result = shutdown;
            return Status::OK();
        }
        if (action == "splitChunk") {
            *result = splitChunk;
            return Status::OK();
        }
        if (action == "splitVector") {
            *result = splitVector;
            return Status::OK();
        }
        if (action == "storageDetails") {
            *result = storageDetails;
            return Status::OK();
        }
        if (action == "top") {
            *result = top;
            return Status::OK();
        }
        if (action == "touch") {
            *result = touch;
            return Status::OK();
        }
        if (action == "unlock") {
            *result = unlock;
            return Status::OK();
        }
        if (action == "update") {
            *result = update;
            return Status::OK();
        }
        if (action == "updateRole") {
            *result = updateRole;
            return Status::OK();
        }
        if (action == "updateUser") {
            *result = updateUser;
            return Status::OK();
        }
        if (action == "validate") {
            *result = validate;
            return Status::OK();
        }
        if (action == "viewRole") {
            *result = viewRole;
            return Status::OK();
        }
        if (action == "viewUser") {
            *result = viewUser;
            return Status::OK();
        }

        return Status(ErrorCodes::FailedToParse,
                      mongoutils::str::stream() << "Unrecognized action privilege string: "
                                                << action,
                      0);
    }

    // Takes an ActionType and returns the string representation
    std::string ActionType::actionToString(const ActionType& action) {
        switch (action.getIdentifier()) {
        case addShardValue:
            return "addShard";
        case anyActionValue:
            return "anyAction";
        case appendOplogNoteValue:
            return "appendOplogNote";
        case applicationMessageValue:
            return "applicationMessage";
        case auditLogRotateValue:
            return "auditLogRotate";
        case authCheckValue:
            return "authCheck";
        case authSchemaUpgradeValue:
            return "authSchemaUpgrade";
        case authenticateValue:
            return "authenticate";
        case changeCustomDataValue:
            return "changeCustomData";
        case changePasswordValue:
            return "changePassword";
        case changeOwnPasswordValue:
            return "changeOwnPassword";
        case changeOwnCustomDataValue:
            return "changeOwnCustomData";
        case cleanupOrphanedValue:
            return "cleanupOrphaned";
        case closeAllDatabasesValue:
            return "closeAllDatabases";
        case collModValue:
            return "collMod";
        case collStatsValue:
            return "collStats";
        case compactValue:
            return "compact";
        case connPoolStatsValue:
            return "connPoolStats";
        case connPoolSyncValue:
            return "connPoolSync";
        case convertToCappedValue:
            return "convertToCapped";
        case cpuProfilerValue:
            return "cpuProfiler";
        case createCollectionValue:
            return "createCollection";
        case createDatabaseValue:
            return "createDatabase";
        case createIndexValue:
            return "createIndex";
        case createRoleValue:
            return "createRole";
        case createUserValue:
            return "createUser";
        case cursorInfoValue:
            return "cursorInfo";
        case dbHashValue:
            return "dbHash";
        case dbStatsValue:
            return "dbStats";
        case diagLoggingValue:
            return "diagLogging";
        case dropAllRolesFromDatabaseValue:
            return "dropAllRolesFromDatabase";
        case dropAllUsersFromDatabaseValue:
            return "dropAllUsersFromDatabase";
        case dropCollectionValue:
            return "dropCollection";
        case dropDatabaseValue:
            return "dropDatabase";
        case dropIndexValue:
            return "dropIndex";
        case dropRoleValue:
            return "dropRole";
        case dropUserValue:
            return "dropUser";
        case emptycappedValue:
            return "emptycapped";
        case enableProfilerValue:
            return "enableProfiler";
        case enableShardingValue:
            return "enableSharding";
        case findValue:
            return "find";
        case flushRouterConfigValue:
            return "flushRouterConfig";
        case fsyncValue:
            return "fsync";
        case getCmdLineOptsValue:
            return "getCmdLineOpts";
        case getLogValue:
            return "getLog";
        case getParameterValue:
            return "getParameter";
        case getShardMapValue:
            return "getShardMap";
        case getShardVersionValue:
            return "getShardVersion";
        case grantRoleValue:
            return "grantRole";
        case grantPrivilegesToRoleValue:
            return "grantPrivilegesToRole";
        case grantRolesToRoleValue:
            return "grantRolesToRole";
        case grantRolesToUserValue:
            return "grantRolesToUser";
        case hostInfoValue:
            return "hostInfo";
        case impersonateValue:
            return "impersonate";
        case indexStatsValue:
            return "indexStats";
        case inprogValue:
            return "inprog";
        case insertValue:
            return "insert";
        case internalValue:
            return "internal";
        case invalidateUserCacheValue:
            return "invalidateUserCache";
        case killCursorsValue:
            return "killCursors";
        case killopValue:
            return "killop";
        case listDatabasesValue:
            return "listDatabases";
        case listShardsValue:
            return "listShards";
        case logRotateValue:
            return "logRotate";
        case moveChunkValue:
            return "moveChunk";
        case netstatValue:
            return "netstat";
        case planCacheIndexFilterValue:
            return "planCacheIndexFilter";
        case planCacheReadValue:
            return "planCacheRead";
        case planCacheWriteValue:
            return "planCacheWrite";
        case reIndexValue:
            return "reIndex";
        case removeValue:
            return "remove";
        case removeShardValue:
            return "removeShard";
        case renameCollectionValue:
            return "renameCollection";
        case renameCollectionSameDBValue:
            return "renameCollectionSameDB";
        case repairDatabaseValue:
            return "repairDatabase";
        case replSetConfigureValue:
            return "replSetConfigure";
        case replSetGetStatusValue:
            return "replSetGetStatus";
        case replSetHeartbeatValue:
            return "replSetHeartbeat";
        case replSetReconfigValue:
            return "replSetReconfig";
        case replSetStateChangeValue:
            return "replSetStateChange";
        case resyncValue:
            return "resync";
        case revokeRoleValue:
            return "revokeRole";
        case revokePrivilegesFromRoleValue:
            return "revokePrivilegesFromRole";
        case revokeRolesFromRoleValue:
            return "revokeRolesFromRole";
        case revokeRolesFromUserValue:
            return "revokeRolesFromUser";
        case serverStatusValue:
            return "serverStatus";
        case setParameterValue:
            return "setParameter";
        case shardCollectionValue:
            return "shardCollection";
        case shardingStateValue:
            return "shardingState";
        case shutdownValue:
            return "shutdown";
        case splitChunkValue:
            return "splitChunk";
        case splitVectorValue:
            return "splitVector";
        case storageDetailsValue:
            return "storageDetails";
        case topValue:
            return "top";
        case touchValue:
            return "touch";
        case unlockValue:
            return "unlock";
        case updateValue:
            return "update";
        case updateRoleValue:
            return "updateRole";
        case updateUserValue:
            return "updateUser";
        case validateValue:
            return "validate";
        case viewRoleValue:
            return "viewRole";
        case viewUserValue:
            return "viewUser";
        default:
            return "";
        }
    }

} // namespace mongo
