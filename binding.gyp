# Here is an incomplete list of things that we need to make a mongo build to get:
# * mongo/base/error_codes.h

{
	"includes": [
		"mongo_src_list.gypi",
	],
	"target_defaults": {
		"configurations": {
			"Debug": {  # TODO: figure out how to get this working!
				"variables": {
					"mongo_build_type": "d"
				}
			},
			"Release": {
				"variables": {
					"mongo_build_type": "normal"
				}
			}
		},
	},
	"targets": [{
	#	"target_name": "clone_mongo",	# TODO: clone mongo into src/third-party.  The the pre-built option in node-gyp might provide a good example.
						# See: https://code.google.com/p/gyp/wiki/GypLanguageSpecification#Actions
						# And maybe: http://stackoverflow.com/questions/18102858/how-to-move-gyp-target-to-a-separate-include-file
	#	"git clone -b v2.6 --depth 2 --single-branch https://github.com/mongodb/mongo src/third-party/mongo"
	#}, {
	#	"target_name": "clone_mongo_cxx_driver", # TODO: for the json->bson parser :)  Assumes it will not be in the mongo src for much longer.
	#}, {
		"target_name": "mungedb-aggregate-native",
		"variables": {
			"mongo_src_dir": "src/third-party/mongo/src/mongo",
			"mongo_dest_dir": "<(LIB_DIR)/mongo",
			"third_party_src_dir": "src/third-party/mongo/src/third_party",
			"third_party_dest_dir": "<(LIB_DIR)/third_party",
			"boost_dir": "<(third_party_dest_dir)/boost",
			"murmurhash3_dir": "<(third_party_dest_dir)/murmurhash3",
		},
		"include_dirs": [
			"src",
			"<(LIB_DIR)",
			"<(boost_dir)",
			"<(mongo_dest_dir)",
		],
		"sources": [
			# Our stuff.
			"src/pipeline.cpp",
			"src/mongo_stubs.cpp",
			"src/mongo-ours/db/interrupt_status_noop.cpp",
			"src/mongo-ours/db/pipeline/document_source_v8.cpp",
			"src/MongoV8Helpers.cpp",

			# Mongo's stuff.
			'<@(mongo_src_files)',

			'<(boost_dir)/libs/thread/src/pthread/thread.cpp',		# TODO: we need to build these via boost.
			'<(boost_dir)/libs/thread/src/pthread/once.cpp',
			'<(boost_dir)/libs/system/src/error_code.cpp',

			"<(murmurhash3_dir)/MurmurHash3.cpp",
		],
		"cflags!": [ "-fno-exceptions", "-fno-rtti" ],
		"cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
		# TEMP TODO: get the compiler flags by using mongo's buildinfo.cpp.
		# TEMP TODO: make sure we are using the same allocator.
		"cflags": [
			"-Wno-ignored-qualifiers", "-Wno-extra",
			"-Wnon-virtual-dtor", "-Woverloaded-virtual", "-fPIC", "-fno-strict-aliasing", "-ggdb", "-pthread", "-Wall", "-Wsign-compare", "-Wno-unknown-pragmas", "-Winvalid-pch", "-pipe", "-Werror", "-O3", "-Wno-unused-local-typedefs", "-Wno-unused-function", "-Wno-deprecated-declarations", "-fno-builtin-memcmp"
		],
		"defines": [ "MONGO_EXPOSE_MACROS=1", "LIBMONGOCLIENT_BUILDING=1" ],
		"defines!": [ "_DEBUG" ],  # Prevent mutexDebugger from being included.  Debug build of mongo doesn't seem to include it.
		"conditions": [
			["OS=='mac'", {
				#"variables": {
				#	"mongo_build_name": "darwin",
				#},
				"xcode_settings": {
					"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
					"GCC_ENABLE_CPP_RTTI": "YES"
				}
			}],
			["OS=='linux'", {
				'defines':[
					"MONGO_HAVE_EXECINFO_BACKTRACE=1",  # TODO: need to only set this if <execinfo.h> is present on the system.
				],
				#"variables": {
				#	"mongo_build_name": "linux",
				#}
			}]
		],
		'copies': [
			# Copying the third party libs to make sure we cannot accidentally include
			# any mongo files.
			{
				'destination': '<(third_party_dest_dir)',
				'files': [
					"<(third_party_src_dir)/murmurhash3",
					"<(third_party_src_dir)/boost",
					],
			},
			# Note: a ton of these just came from lite_parsed_query.*
			{
				'destination': '<(mongo_dest_dir)',
				'files': [
					"<(mongo_src_dir)/base",
					'<(mongo_src_dir)/bson',
					'<(mongo_src_dir)/pch.h',
					'<(mongo_src_dir)/pch.cpp',
					'<(mongo_src_dir)/platform',
					'<(mongo_src_dir)/logger',			# Do we really need all of this?
					],
			},
			{
				'destination': '<(mongo_dest_dir)/db',
				'files': [
						'<(mongo_src_dir)/db/pipeline',
						'<(mongo_src_dir)/db/server_options.h',
						'<(mongo_src_dir)/db/server_options.cpp',
						#'<(mongo_src_dir)/db/client.cpp',
					],
			},
			{
				"destination": "<(mongo_dest_dir)/util",
				"files": [
						"<(mongo_src_dir)/util/intrusive_counter.h",
						"<(mongo_src_dir)/util/intrusive_counter.cpp",
						"<(mongo_src_dir)/util/exit_code.h",
						"<(mongo_src_dir)/util/assert_util.h",
						"<(mongo_src_dir)/util/debug_util.h",
						"<(mongo_src_dir)/util/debug_util.cpp",
						"<(mongo_src_dir)/util/goodies.h",
						"<(mongo_src_dir)/util/time_support.h",
						"<(mongo_src_dir)/util/time_support.cpp",
						"<(mongo_src_dir)/util/allocator.h",
						"<(mongo_src_dir)/util/log.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.cpp",
						"<(mongo_src_dir)/util/heapcheck.h",
						"<(mongo_src_dir)/util/hex.h",
						"<(mongo_src_dir)/util/hex.cpp",
						"<(mongo_src_dir)/util/bufreader.h",

						# Compile.
						"<(mongo_src_dir)/util/timer.h",
						"<(mongo_src_dir)/util/timer-inl.h",
						"<(mongo_src_dir)/util/timer-generic-inl.h",
						"<(mongo_src_dir)/util/background.h",
						"<(mongo_src_dir)/util/string_map.h",
						"<(mongo_src_dir)/util/unordered_fast_key_table.h",
						"<(mongo_src_dir)/util/unordered_fast_key_table_internal.h",
						"<(mongo_src_dir)/util/text.h",
						"<(mongo_src_dir)/util/base64.h",
						"<(mongo_src_dir)/util/safe_num.h",
						"<(mongo_src_dir)/util/safe_num-inl.h",
						# TODO this is required by document_source_cursor.cpp, db/db_client.h
						#            and we should mock out the functionality we need.
						"<(mongo_src_dir)/util/paths.h",		# Seems like this is a reasonable dependency.
						"<(mongo_src_dir)/util/progress_meter.h",
						# --- document_source_cursor.cpp stuff to remove.
						"<(mongo_src_dir)/util/stringutils.h",
						"<(mongo_src_dir)/util/embedded_builder.h",
						"<(mongo_src_dir)/util/md5.h",
						"<(mongo_src_dir)/util/md5.hpp",
						"<(mongo_src_dir)/util/startup_test.h",		# This seems like an odd requirement for jsobj.cpp
						"<(mongo_src_dir)/util/stacktrace.h",			# Needed by DBException

						# Load.
						"<(mongo_src_dir)/util/assert_util.cpp",	# Needed to fix missing DBException on load.
						"<(mongo_src_dir)/util/stacktrace.cpp",	# Needed to fix missing DBException on load.
						"<(mongo_src_dir)/util/startup_test.cpp",	# Needed to fix missing StartupTest on load.
						"<(mongo_src_dir)/util/background.cpp",	# PeriodicTask
						"<(mongo_src_dir)/util/password_digest.h",
						"<(mongo_src_dir)/util/util.cpp",
						"<(mongo_src_dir)/util/mmap.h",
						#"<(mongo_src_dir)/util/admin_access.h",		# client.cpp
						#"<(mongo_src_dir)/util/file_allocator.h",		# client.cpp
						"<(mongo_src_dir)/util/log.cpp",		# Load.
						"<(mongo_src_dir)/util/queue.h",		# rs.cpp.
						"<(mongo_src_dir)/util/timer.cpp",		# Load: _countsPerSecond.
						"<(mongo_src_dir)/util/map_util.h",		# rotatable_file_manager.cpp
						"<(mongo_src_dir)/util/fail_point_service.h",		# sock.cpp
						"<(mongo_src_dir)/util/fail_point_registry.h",		# sock.cpp
						"<(mongo_src_dir)/util/fail_point.h",		# sock.cpp
						"<(mongo_src_dir)/util/base64.cpp",		# json.cpp
						"<(mongo_src_dir)/util/fail_point.cpp",		# FailPoint().
						"<(mongo_src_dir)/util/fail_point_service.cpp",		# Run time, dependency graph.
						"<(mongo_src_dir)/util/fail_point_registry.cpp",		# Run time, dependency graph.
					]
			},
			{
				"destination": "<(mongo_dest_dir)/util/concurrency",
				"files": [
						"<(mongo_src_dir)/util/concurrency/thread_name.h",
						"<(mongo_src_dir)/util/concurrency/thread_name.cpp",
						"<(mongo_src_dir)/util/concurrency/mutex.h",
						"<(mongo_src_dir)/util/concurrency/threadlocal.h",
						"<(mongo_src_dir)/util/concurrency/ticketholder.h",
						# TODO this is required by document_source_cursor.cpp, db/d_concurrency.h:41:43 |  db/curop.h
						#            and we should mock out the functionality we need.
						"<(mongo_src_dir)/util/concurrency/rwlock.h",
						"<(mongo_src_dir)/util/concurrency/rwlockimpl.h",
						"<(mongo_src_dir)/util/concurrency/simplerwlock.h",
						"<(mongo_src_dir)/util/concurrency/spin_lock.h",
						# --- document_source_cursor.cpp stuff to remove.
						#"<(mongo_src_dir)/util/concurrency/list.h",								# client.cpp
						#"<(mongo_src_dir)/util/concurrency/race.h",							# client.cpp
						#"<(mongo_src_dir)/util/concurrency/mutexdebugger.h",	# client.cpp
						#"<(mongo_src_dir)/util/concurrency/thread_pool.h",	# client.cpp
						#"<(mongo_src_dir)/util/concurrency/msg.h",	# client.cpp
						#"<(mongo_src_dir)/util/concurrency/task.h",	# client.cpp
						"<(mongo_src_dir)/util/concurrency/value.h",	# sock.cpp
						"<(mongo_src_dir)/util/concurrency/spin_lock.cpp",	# SpinLock()
					]
			},
			{
				"destination": "<(mongo_dest_dir)/util/net",
				"files": [
						# Compile.
						"<(mongo_src_dir)/util/net/hostandport.h",
						"<(mongo_src_dir)/util/net/message.h",
						"<(mongo_src_dir)/util/net/message.cpp",
						"<(mongo_src_dir)/util/net/listen.h",
						"<(mongo_src_dir)/util/net/sock.h",
						"<(mongo_src_dir)/util/net/message_port.h",
						"<(mongo_src_dir)/util/net/message_port.cpp",

						# Load.
						"<(mongo_src_dir)/util/net/ssl_manager.h",
						"<(mongo_src_dir)/util/net/ssl_options.h",
						"<(mongo_src_dir)/util/net/sock.cpp",		# vtable for SocketException
						"<(mongo_src_dir)/util/net/socket_poll.h",		# sock.cpp
						]
			},
			{
				"destination": "<(mongo_dest_dir)/util/mongoutils",
				"files": [
						"<(mongo_src_dir)/util/mongoutils/str.h",

						# Loading.
						"<(mongo_src_dir)/util/mongoutils/checksum.h",
						"<(mongo_src_dir)/util/mongoutils/html.h",	# ramlog.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/client",
				"files": [
						'<(mongo_src_dir)/client/constants.h',
						'<(mongo_src_dir)/client/undef_macros.h',
						'<(mongo_src_dir)/client/redef_macros.h',
						'<(mongo_src_dir)/client/export_macros.h',

						# compile.
						'<(mongo_src_dir)/client/connpool.h',
						'<(mongo_src_dir)/client/dbclientinterface.h',
						'<(mongo_src_dir)/client/dbclientcursor.h',
						'<(mongo_src_dir)/client/syncclusterconnection.h',
						'<(mongo_src_dir)/client/dbclient_rs.h',

						# Load.
						'<(mongo_src_dir)/client/replica_set_monitor.h',
						#'<(mongo_src_dir)/client/replica_set_monitor.cpp',
						'<(mongo_src_dir)/client/replica_set_monitor_internal.h',
						"<(mongo_src_dir)/client/dbclientcursor.cpp",	# DBClientCursor
						"<(mongo_src_dir)/client/connpool.cpp",				# pool
						'<(mongo_src_dir)/client/dbclient.cpp',
						'<(mongo_src_dir)/client/sasl_client_authenticate.h',
						'<(mongo_src_dir)/client/sasl_client_authenticate.cpp',
					]
			},
			{
				"destination": "<(mongo_dest_dir)/base",
				"files": [
						'<(mongo_src_dir)/../../build/linux2/normal/mongo/base/error_codes.h',		# TODO: WE NED TO GENERATE THIS.
						'<(mongo_src_dir)/../../build/linux2/normal/mongo/base/error_codes.cpp',
					]
			},
			{
				"destination": "<(mongo_dest_dir)/s",
				"files": [
						# Compile.
						'<(mongo_src_dir)/s/collection_metadata.h',
						'<(mongo_src_dir)/s/chunk_version.h',
						'<(mongo_src_dir)/s/bson_serializable.h',
						'<(mongo_src_dir)/s/range_arithmetic.h',
						'<(mongo_src_dir)/s/type_chunk.h',
						'<(mongo_src_dir)/s/shard.h',
						'<(mongo_src_dir)/s/strategy.h',
						'<(mongo_src_dir)/s/chunk.h',
						'<(mongo_src_dir)/s/distlock.h',
						'<(mongo_src_dir)/s/shardkey.h',
						'<(mongo_src_dir)/s/shard_key_pattern.h',
						'<(mongo_src_dir)/s/request.h',
						'<(mongo_src_dir)/s/config.h',

						# Another document_source_cursor.cpp thing to try to remove.
						'<(mongo_src_dir)/s/d_logic.h',
						'<(mongo_src_dir)/s/stale_exception.h',
						# End Another document_source_cursor.cpp thing to try to rem ove.
						'<(mongo_src_dir)/s/d_state.cpp',		# Load: shardingState.
						'<(mongo_src_dir)/s/metadata_loader.h',		# d_state.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db",
				"files": [
						# include.
						"<(mongo_src_dir)/db/jsobj.h",
						"<(mongo_src_dir)/db/jsobj.cpp",
						"<(mongo_src_dir)/db/dbmessage.h",
						"<(mongo_src_dir)/db/dbmessage.cpp",

						# compile.
						"<(mongo_src_dir)/db/interrupt_status.h",
						"<(mongo_src_dir)/db/namespace_string.h",
						"<(mongo_src_dir)/db/namespace_string-inl.h",
						"<(mongo_src_dir)/db/clientcursor.h",
						"<(mongo_src_dir)/db/diskloc.h",
						"<(mongo_src_dir)/db/keypattern.h",
						"<(mongo_src_dir)/db/field_ref.h",
						"<(mongo_src_dir)/db/matcher",
						"<(mongo_src_dir)/db/invalidation_type.h",
						"<(mongo_src_dir)/db/field_ref_set.h",
						"<(mongo_src_dir)/db/matcher.h",
						"<(mongo_src_dir)/db/projection.h",
						"<(mongo_src_dir)/db/sorter",
						"<(mongo_src_dir)/db/json.h",

						# TODO this is required by document_source_cursor.cpp:33:31: fatal error: mongo/db/instance.h
						#            and we should mock out the functionality we need.
						"<(mongo_src_dir)/db/instance.h",
						"<(mongo_src_dir)/db/client.h",
						"<(mongo_src_dir)/db/client_basic.h",
						"<(mongo_src_dir)/db/d_concurrency.h",
						"<(mongo_src_dir)/db/lockstat.h",
						"<(mongo_src_dir)/db/lasterror.h",
						"<(mongo_src_dir)/db/lockstate.h",
						"<(mongo_src_dir)/db/storage_options.h",
						"<(mongo_src_dir)/db/curop-inl.h",
						"<(mongo_src_dir)/db/curop.h",
						# --- document_source_cursor.cpp stuff to remove.
						"<(mongo_src_dir)/db/commands.h", # TODO this is required by pipeline.cpp Mock or make it optional.

						# Load
						"<(mongo_src_dir)/db/storage_options.cpp", # storageOptions
						"<(mongo_src_dir)/db/server_parameters.h", # Needed by storage_options.cinepp
						"<(mongo_src_dir)/db/server_parameters.cpp", # Needed by storage_options.cpp
						"<(mongo_src_dir)/db/server_parameters_inline.h", # Needed by storage_options.cpp
						"<(mongo_src_dir)/db/field_parser.h",
						"<(mongo_src_dir)/db/field_parser-inl.h",
						#"<(mongo_src_dir)/db/db.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/index_names.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/dur.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/update_index_data.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/pdfile.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/pdfile_version.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/kill_current_op.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/dbwebserver.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/pagefault.h",	# client.cpp again.
						#"<(mongo_src_dir)/db/client_basic.cpp",
						"<(mongo_src_dir)/db/commands.cpp",	# Load.
						"<(mongo_src_dir)/db/audit.h",		# commands.cpp
						"<(mongo_src_dir)/db/lasterror.cpp",		# Load.
						"<(mongo_src_dir)/db/dbhelpers.h",		# rs.cpp.
						"<(mongo_src_dir)/db/wire_version.h",		# d_state.cpp.
						"<(mongo_src_dir)/db/json.cpp",				# fromjson().
					]
			},
			{
				# Part of mongoscore lib and used by pipeline.cpp.
				"destination": "<(mongo_dest_dir)/db/query",
				"files": [
						# include.
						"<(mongo_src_dir)/db/query/lite_parsed_query.cpp",
						"<(mongo_src_dir)/db/query/lite_parsed_query.h",

						# Compile
						"<(mongo_src_dir)/db/query/runner.h",
						"<(mongo_src_dir)/db/query/canonical_query.h",
						"<(mongo_src_dir)/db/query/parsed_projection.h",
						"<(mongo_src_dir)/db/query/find_constants.h",				# Remove? included by document_source_cursor.cpp
						"<(mongo_src_dir)/db/query/type_explain.h",				# Try to remove.  included by document_source_cursor.cpp
						"<(mongo_src_dir)/db/query/type_explain.cpp",
						#"<(mongo_src_dir)/db/query/plan_cache.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/stage_types.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/index_tag.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/lru_key_value.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/query_planner_params.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/index_entry.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/query_knobs.h",		# client.cpp
						#"<(mongo_src_dir)/db/query/query_settings.h",		# client.cpp
					]
			},
			# TODO this is required by document_source_cursor.cpp, mongo/db/client.h
			#            and we should mock out the functionality we need.
			{
				"destination": "<(mongo_dest_dir)/db/stats",
				"files": [
						"<(mongo_src_dir)/db/stats/top.h",
					]
			},
			# --- document_source_cursor.cpp stuff to remove.
			# TODO this is required by document_source_cursor.cpp, mongo/db/curop.h
			#            and we should mock out the functionality we need.
			{
				"destination": "<(mongo_dest_dir)/db/structure/catalog",
				"files": [
					# Loading.
						"<(mongo_src_dir)/db/structure/catalog/namespace.h",
						"<(mongo_src_dir)/db/structure/catalog/namespace-inl.h",
						#"<(mongo_src_dir)/db/structure/catalog/namespace_details.h",  # client.cpp
						#"<(mongo_src_dir)/db/structure/catalog/index_details.h",  # client.cpp
						#"<(mongo_src_dir)/db/structure/catalog/namespace_index.h",	# client.cpp
						#"<(mongo_src_dir)/db/structure/catalog/hashtab.h",	# client.cpp
						#"<(mongo_src_dir)/db/structure/catalog/namespace_details-inl.h",  # client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/structure/btree",
				"files": [
						#"<(mongo_src_dir)/db/structure/btree/key.h",	# client.cpp ?
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/repl",
				"files": [
						#"<(mongo_src_dir)/db/repl/rs.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/oplogreader.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/rs_config.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/health.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/rs_exception.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/rs_member.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/rs_sync.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/sync.h",			# client.cpp
						#"<(mongo_src_dir)/db/repl/sync_source_feedback.h",			# client.cpp
						"<(mongo_src_dir)/db/repl/rs.cpp",			# Load.
						"<(mongo_src_dir)/db/repl/bgsync.h",			# rs.cpp.
						"<(mongo_src_dir)/db/repl/connections.h",			# rs.cpp.
						"<(mongo_src_dir)/db/repl/oplog.h",			# rs.cpp.
						"<(mongo_src_dir)/db/repl/replication_server_status.h",			# rs.cpp.
						"<(mongo_src_dir)/db/repl/is_master.h",			# d_state.cpp.
						"<(mongo_src_dir)/db/repl/master_slave.h",			# d_state.cpp.
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/index",
				"files": [
						#"<(mongo_src_dir)/db/index/index_descriptor.h",		# client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/exec",
				"files": [
						#"<(mongo_src_dir)/db/exec/collection_scan_common.h",		# client.cpp
						#"<(mongo_src_dir)/db/exec/plan_stats.h",		# client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/geo",
				"files": [
						#"<(mongo_src_dir)/db/geo/hash.h",		# client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/structure",
				"files": [
						#"<(mongo_src_dir)/db/structure/record_store.h",		# client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/db/storage",
				"files": [
						# Loading
						#"<(mongo_src_dir)/db/storage/durable_mapped_file.h",	# client.cpp
						#"<(mongo_src_dir)/db/storage/extent_manager.h",				# client.cpp
						#"<(mongo_src_dir)/db/storage/record.h",				# client.cpp
						#"<(mongo_src_dir)/db/storage/extent.h",				# client.cpp
						#"<(mongo_src_dir)/db/storage/data_file.h",				# client.cpp
					]
			},
			# TODO this is required by pipeline.cpp Mock out the bits we need
			#				 or split pipelne.cpp up (might have our own version).
			{
				"destination": "<(mongo_dest_dir)/db/auth",
				"files": [
						# Compile.
						"<(mongo_src_dir)/db/auth/action_set.h",
						"<(mongo_src_dir)/db/auth/privilege.h",
						"<(mongo_src_dir)/../../build/linux2/normal/mongo/db/auth/action_type.h",		# TODO FIX ME!!!  Need to have the mongo build generate this.
						"<(mongo_src_dir)/db/auth/privilege_parser.h",
						"<(mongo_src_dir)/db/auth/resource_pattern.h",

						# Load.
						"<(mongo_src_dir)/../../build/linux2/normal/mongo/db/auth/action_type.cpp",	# TODO FIX ME!!!  Need to have the mongo build generate this.
						#"<(mongo_src_dir)/db/auth/authorization_manager_global.h", # Most of these are included by client.cpp
						#"<(mongo_src_dir)/db/auth/authorization_manager.h",
						#"<(mongo_src_dir)/db/auth/role_graph.h",
						#"<(mongo_src_dir)/db/auth/role_name.h",
						#"<(mongo_src_dir)/db/auth/user.h",
						#"<(mongo_src_dir)/db/auth/user_name.h",
						#"<(mongo_src_dir)/db/auth/user_name_hash.h",
						#"<(mongo_src_dir)/db/auth/authorization_session.h",
						#"<(mongo_src_dir)/db/auth/authz_session_external_state.h",
						#"<(mongo_src_dir)/db/auth/user_set.h",
						#"<(mongo_src_dir)/db/auth/authz_session_external_state_d.h",
						#"<(mongo_src_dir)/db/auth/authz_session_external_state_server_common.h",
						#"<(mongo_src_dir)/db/auth/authentication_session.h",	# client_basic.cpp
						"<(mongo_src_dir)/db/auth/security_key.h",	# rs.cpp
					]
			},			# --- pipeline.cpp stuff to mock.
			# TODO this is required by pipeline_d.cpp Mock out the bits we need
			#				 or split pipelne.cpp up (might have our own version).
			{
				"destination": "<(mongo_dest_dir)/db/catalog",
				"files": [
						#"<(mongo_src_dir)/db/catalog/database_holder.h",  # client.cpp
						#"<(mongo_src_dir)/db/catalog/database.h",  # client.cpp
						#"<(mongo_src_dir)/db/catalog/collection_info_cache.h",  # client.cpp
			#			"<(mongo_src_dir)/db/catalog/collection.h",
			#			"<(mongo_src_dir)/db/catalog/collection_cursor_cache.h",
			#			"<(mongo_src_dir)/db/catalog/index_catalog.h",
			#			"<(mongo_src_dir)/db/catalog/index_catalog_entry.h",
			#			"<(mongo_src_dir)/db/catalog/index_pregen.h",
					]
			},		# pipeline_d.cpp stuff to get rid of.
			{
				"destination": "<(mongo_dest_dir)/scripting",
				"files": [
						"<(mongo_src_dir)/scripting/v8_utils.h",
						#"<(mongo_src_dir)/scripting/engine.h",		# client.cpp
					]
			},
			{
				"destination": "<(mongo_dest_dir)/logger",
				"files": [
						# Load: ~RotatableFileManager()
						'<(mongo_src_dir)/logger/rotatable_file_manager.cpp',
						'<(mongo_src_dir)/logger/rotatable_file_manager.h',
						'<(mongo_src_dir)/logger/ramlog.cpp',	# RamLog::get()
						'<(mongo_src_dir)/logger/ramlog.h',
					]
			},
		]
	}]  # End targets.
}
