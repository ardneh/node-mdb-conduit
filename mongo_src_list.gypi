{
"variables":{
"mongo_src_files": [
#"build/Release/obj.target/mongo/base/counter_test.cpp",
"build/Release/obj.target/mongo/base/global_initializer.cpp",
"build/Release/obj.target/mongo/base/global_initializer_registerer.cpp",
"build/Release/obj.target/mongo/base/init.cpp",
"build/Release/obj.target/mongo/base/initializer.cpp",
"build/Release/obj.target/mongo/base/initializer_context.cpp",
"build/Release/obj.target/mongo/base/initializer_dependency_graph.cpp",
#"build/Release/obj.target/mongo/base/initializer_dependency_graph_test.cpp",
#"build/Release/obj.target/mongo/base/initializer_test.cpp",
"build/Release/obj.target/mongo/base/make_string_vector.cpp",
#"build/Release/obj.target/mongo/base/owned_pointer_map_test.cpp",
#"build/Release/obj.target/mongo/base/owned_pointer_vector_test.cpp",
"build/Release/obj.target/mongo/base/parse_number.cpp",
#"build/Release/obj.target/mongo/base/parse_number_test.cpp",
"build/Release/obj.target/mongo/base/status.cpp",
#"build/Release/obj.target/mongo/base/status_test.cpp",
"build/Release/obj.target/mongo/base/string_data.cpp",
#"build/Release/obj.target/mongo/base/string_data_test.cpp",
"build/Release/obj.target/mongo/base/error_codes.cpp",
#"build/Release/obj.target/mongo/bson/bsondemo/bsondemo.cpp",
"build/Release/obj.target/mongo/bson/mutable/document.cpp",
"build/Release/obj.target/mongo/bson/mutable/element.cpp",
#"build/Release/obj.target/mongo/bson/mutable/mutable_bson_algo_test.cpp",
#"build/Release/obj.target/mongo/bson/mutable/mutable_bson_test.cpp",
#"build/Release/obj.target/mongo/bson/mutable/mutable_bson_test_utils.cpp",
"build/Release/obj.target/mongo/bson/util/bson_extract.cpp",
#"build/Release/obj.target/mongo/bson/util/bson_extract_test.cpp",
#"build/Release/obj.target/mongo/bson/util/builder_test.cpp",
#"build/Release/obj.target/mongo/bson/bson_field_test.cpp",
#"build/Release/obj.target/mongo/bson/bson_obj_test.cpp",
"build/Release/obj.target/mongo/bson/bson_validate.cpp",
#"build/Release/obj.target/mongo/bson/bson_validate_test.cpp",
#"build/Release/obj.target/mongo/bson/bsonobjbuilder_test.cpp",
"build/Release/obj.target/mongo/bson/oid.cpp",
"build/Release/obj.target/mongo/bson/optime.cpp",
"build/Release/obj.target/mongo/pch.cpp",
#"build/Release/obj.target/mongo/platform/atomic_word_test.cpp",
"build/Release/obj.target/mongo/platform/backtrace.cpp",
#"build/Release/obj.target/mongo/platform/bits_test.cpp",
"build/Release/obj.target/mongo/platform/posix_fadvise.cpp",
"build/Release/obj.target/mongo/platform/process_id.cpp",
#"build/Release/obj.target/mongo/platform/process_id_test.cpp",
"build/Release/obj.target/mongo/platform/random.cpp",
#"build/Release/obj.target/mongo/platform/random_test.cpp",
"build/Release/obj.target/mongo/platform/strcasestr.cpp",
"build/Release/obj.target/mongo/logger/console.cpp",
#"build/Release/obj.target/mongo/logger/console_test.cpp",
"build/Release/obj.target/mongo/logger/log_manager.cpp",
"build/Release/obj.target/mongo/logger/log_severity.cpp",
#"build/Release/obj.target/mongo/logger/log_test.cpp",
"build/Release/obj.target/mongo/logger/logger.cpp",
"build/Release/obj.target/mongo/logger/logstream_builder.cpp",
"build/Release/obj.target/mongo/logger/message_event_utf8_encoder.cpp",
"build/Release/obj.target/mongo/logger/message_log_domain.cpp",
"build/Release/obj.target/mongo/logger/ramlog.cpp", # Includes util/mongoutils/html.h, trying to skip.  RamLog::get() needed at runtime.
"build/Release/obj.target/mongo/logger/rotatable_file_manager.cpp", # Includes mongo/util/map_util.h, trying to skip.
#"build/Release/obj.target/mongo/logger/rotatable_file_writer.cpp", # Part of rotatable_file_manager.cpp ?
#"build/Release/obj.target/mongo/logger/rotatable_file_writer_test.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_add_to_set.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_avg.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_first.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_last.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_min_max.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_push.cpp",
"build/Release/obj.target/mongo/db/pipeline/accumulator_sum.cpp",
"build/Release/obj.target/mongo/db/pipeline/dependencies.cpp",
"build/Release/obj.target/mongo/db/pipeline/document.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_bson_array.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_command_shards.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_cursor.cpp", # This pulled in a ton of stuff, including a bunch of server only things.  Try to get rid of it or split it up to client only bits.
"build/Release/obj.target/mongo/db/pipeline/document_source_geo_near.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_group.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_limit.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_match.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_merge_cursors.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_out.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_project.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_redact.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_skip.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_sort.cpp",
"build/Release/obj.target/mongo/db/pipeline/document_source_unwind.cpp",
"build/Release/obj.target/mongo/db/pipeline/expression.cpp",
"build/Release/obj.target/mongo/db/pipeline/field_path.cpp",
"build/Release/obj.target/mongo/db/pipeline/pipeline.cpp",
# "build/Release/obj.target/mongo/db/pipeline/pipeline_d.cpp",  # Includes a bunch of server side stuff, try to not include it.
"build/Release/obj.target/mongo/db/pipeline/value.cpp",
"build/Release/obj.target/mongo/db/jsobj.cpp",
"build/Release/obj.target/mongo/db/dbmessage.cpp",
"build/Release/obj.target/mongo/db/query/lite_parsed_query.cpp",
"build/Release/obj.target/mongo/util/intrusive_counter.cpp",
"build/Release/obj.target/mongo/util/debug_util.cpp",
"build/Release/obj.target/mongo/util/time_support.cpp",
#"build/Release/obj.target/mongo/util/thread_name.cpp", 	# Causing linker errors.
"build/Release/obj.target/mongo/util/hex.cpp",
"build/Release/obj.target/mongo/util/concurrency/thread_name.cpp",
#"build/Release/obj.target/mongo/util/net/message.cpp",		# Doesn't seem like we should need this.
#"build/Release/obj.target/mongo/util/net/message_port.cpp",	# Doesn't seem like we should need this.

# Everything below was added to resolve missing symbol errors at runtime.
"build/Release/obj.target/mongo/util/assert_util.cpp",		# DBException
"build/Release/obj.target/mongo/util/stacktrace.cpp",		# Needed by DBException.
"build/Release/obj.target/mongo/util/startup_test.cpp",		# StartupTest.
"build/Release/obj.target/mongo/db/storage_options.cpp",	# storageGlobalParams
"build/Release/obj.target/mongo/db/server_parameters.cpp",	# Needed by storage_options.cpp
#"build/Release/obj.target/mongo/client/replica_set_monitor.cpp",	# Needed by storage_options.cpp
"build/Release/obj.target/mongo/db/auth/action_type.cpp",		#
#"build/Release/obj.target/mongo/db/auth/action_set.cpp",		#
"build/Release/obj.target/mongo/client/dbclientcursor.cpp",	# DBClientCursor
"build/Release/obj.target/mongo/client/connpool.cpp",		# pool
"build/Release/obj.target/mongo/util/background.cpp",		# PeriodicTask
#"build/Release/obj.target/mongo/util/net/ssl_manager.cpp",	# needed by background.cpp
"build/Release/obj.target/mongo/client/dbclient.cpp",		#
"build/Release/obj.target/mongo/db/query/type_explain.cpp",
"build/Release/obj.target/mongo/client/sasl_client_authenticate.cpp",
"build/Release/obj.target/mongo/util/util.cpp",		# PeriodicTask
#"build/Release/obj.target/mongo/db/client.cpp",		#
#"build/Release/obj.target/mongo/db/client_basic.cpp",		#
#"build/Release/obj.target/mongo/db/commands.cpp",		# Load.
#"build/Release/obj.target/mongo/db/lasterror.cpp",		# Load.
"build/Release/obj.target/mongo/util/log.cpp",		# Load.
#"build/Release/obj.target/mongo/db/repl/rs.cpp",		# Load.
#"build/Release/obj.target/mongo/s/d_state.cpp",		# Load: shardingState
"build/Release/obj.target/mongo/db/server_options.cpp",		# Load: serverGlobalOptions.
"build/Release/obj.target/mongo/util/timer.cpp",		# Timer::_countsPerSecond
"build/Release/obj.target/mongo/util/net/sock.cpp",		# vtable SocketException
"build/Release/obj.target/mongo/db/json.cpp",    # fromjson()
"build/Release/obj.target/mongo/util/base64.cpp",		# json.cpp
"build/Release/obj.target/mongo/util/fail_point.cpp",		# FailPoint()
"build/Release/obj.target/mongo/util/concurrency/spin_lock.cpp",  # SpinLock()
]
}
}
