cmake_minimum_required (VERSION 3.5)

project ("NearRPC")

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

set(CMAKE_C_ARCHIVE_FINISH true)
set(CMAKE_CXX_ARCHIVE_FINISH true)

set(SOURSCES
    NearPlugin/Client.cpp
    NearPlugin/EdKeys.cpp
    NearPlugin/GRPC_Client.cpp
    NearPlugin/src/add_scalar.c
    NearPlugin/src/fe.c
    NearPlugin/src/ge.c
    NearPlugin/src/key_exchange.c
    NearPlugin/src/keypair.c
    NearPlugin/src/sc.c
    NearPlugin/src/seed.c
    NearPlugin/src/sha512.c
    NearPlugin/src/sign.c
    NearPlugin/src/verify.c
    NearPlugin/protocol/auth.pb.cc
    NearPlugin/protocol/auth.grpc.pb.cc
)
set(HEADERS
    NearPlugin/Client.h
    NearPlugin/EdKeys.h
    NearPlugin/GRPC_Client.h
    NearPlugin/src/ed25519.h
    NearPlugin/src/fe.h
    NearPlugin/src/fixedint.h
    NearPlugin/src/ge.h
    NearPlugin/src/precomp_data.h
    NearPlugin/src/sc.h
    NearPlugin/src/sha512.h
    NearPlugin/protocol/auth.pb.h
    NearPlugin/protocol/auth.grpc.pb.h
)

set(GRPCLIB_ROOT ${CMAKE_INSTALL_PREFIX})

include_directories(${CMAKE_INSTALL_PREFIX}/include/)
set(LIBS
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_cord.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_str_format_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_debugging_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_strerror.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_strings.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_strings_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_status.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_statusor.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_bad_optional_access.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_stacktrace.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_symbolize.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_int128.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_bad_variant_access.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_city.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_civil_time.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_cord_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_cordz_functions.a
    ${CMAKE_INSTALL_PREFIX}/lib/libaddress_sorting.a
    ${CMAKE_INSTALL_PREFIX}/lib/libcares.a
    ${CMAKE_INSTALL_PREFIX}/lib/libgpr.a
    ${CMAKE_INSTALL_PREFIX}/lib/libgrpc.a
    ${CMAKE_INSTALL_PREFIX}/lib/libgrpc++.a
    ${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a
    ${CMAKE_INSTALL_PREFIX}/lib/libupb.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_base.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_malloc_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_raw_logging_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_spinlock_wait.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_throw_delegate.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_time.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_time_zone.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_graphcycles_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_synchronization.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_cordz_handle.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_cordz_info.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_demangle_internal.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_exponential_biased.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_hash.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_hashtablez_sampler.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_log_severity.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_low_level_hash.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_distributions.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_seed_sequences.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_platform.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_seed_gen_exception.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_seed_material.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_randen_slow.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_randen_hwaes_impl.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_randen_hwaes.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_randen.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_random_internal_pool_urbg.a
    ${CMAKE_INSTALL_PREFIX}/lib/libre2.a
    ${CMAKE_INSTALL_PREFIX}/lib/libabsl_raw_hash_set.a
)

add_library(NearRPC STATIC ${SOURSCES} ${HEADERS} ${LIBS})

target_link_libraries(NearRPC ${LIBS})


set_target_properties(
    NearRPC PROPERTIES
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "YES"
    CXX_STANDARD 17
    CXX_EXTENSIONS ON
)