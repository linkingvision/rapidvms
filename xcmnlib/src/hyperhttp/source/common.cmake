
# This utility function starts from the directory containing the current CMakeLists.txt
# and works backward up the tree looking for "devel_artifacts". If found, the path to
# devel_artifacts is returned in result.
function(find_devel_artifacts devel_artifacts_path)
    set(native_artifact_path ${CMAKE_CURRENT_SOURCE_DIR})
    file(TO_CMAKE_PATH ${native_artifact_path} internal_artifact_path)
    set(found "false")
    while(${found} STREQUAL "false")
        # First, see if we have any more "/", if we don't then further splitting
        # will not work so we should bail.
        string(FIND ${internal_artifact_path} "/" pos)
        if(${pos} EQUAL -1)
            message(FATAL_ERROR "Unable to find devel_artifacts!")
        endif(${pos} EQUAL -1)
        set(potential_path "${internal_artifact_path}/devel_artifacts")
        file(TO_NATIVE_PATH ${potential_path} potential_native_path)
        if(EXISTS ${potential_native_path})
            set(found "true")
        else(EXISTS ${potential_native_path})
            string(REPLACE "/" ";" path_list ${internal_artifact_path})
            list(REMOVE_AT path_list -1)
            string(REPLACE ";" "/" internal_artifact_path "${path_list}")
        endif(EXISTS ${potential_native_path})
    endwhile(${found} STREQUAL "false")
    set(devel_artifacts_path ${potential_path} PARENT_SCOPE)
# leaving this here as an example if you ever need a "native path"
#    file(TO_NATIVE_PATH ${potential_path} native_artifact_path)
#    set(devel_artifacts_path ${native_artifact_path} PARENT_SCOPE)
endfunction(find_devel_artifacts devel_artifacts_path)
find_devel_artifacts(devel_artifacts_path)
