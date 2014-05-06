{
  'target_defaults': {
    'dependencies': [
      '<(platform_root)/metrics/libmetrics-<(libbase_ver).gyp:libmetrics-<(libbase_ver)',
    ],
    'variables': {
      'deps': [
        'dbus-1',
        'libchrome-<(libbase_ver)',
        'libchrome-test-<(libbase_ver)',
        'libcurl',
      ],
    },
    # TODO(sosa): Remove gflags: crbug.com/356745.
    'link_settings': {
      'libraries': [
        '-lgflags',
        '-lbase-dbus_test_support-<(libbase_ver)',
      ],
    },
    'cflags_cc': [
      '-std=gnu++11',
    ],
  },
  'targets': [
    {
      'target_name': 'buffet_common',
      'type': 'static_library',
      'sources': [
        'any.cc',
        'async_event_sequencer.cc',
        'data_encoding.cc',
        'dbus_constants.cc',
        'dbus_utils.cc',
        'device_registration_info.cc',
        'error.cc',
        'exported_object_manager.cc',
        'exported_property_set.cc',
        'http_request.cc',
        'http_connection_curl.cc',
        'http_transport_curl.cc',
        'http_utils.cc',
        'manager.cc',
        'mime_utils.cc',
        'storage_impls.cc',
        'string_utils.cc',
        'url_utils.cc'
      ],
    },
    {
      'target_name': 'buffet',
      'type': 'executable',
      'sources': [
        'main.cc',
      ],
      'dependencies': [
        'buffet_common',
      ],
    },
    {
      'target_name': 'buffet_client',
      'type': 'executable',
      'sources': [
        'buffet_client.cc',
        'dbus_constants.cc',
      ],
      'dependencies': [
        'buffet_common',
      ],
    },
    {
      'target_name': 'buffet_testrunner',
      'type': 'executable',
      'dependencies': [
        'buffet_common',
      ],
      'includes': ['../common-mk/common_test.gypi'],
      'sources': [
        'any_unittest.cc',
        'any_internal_impl_unittest.cc',
        'async_event_sequencer_unittest.cc',
        'buffet_testrunner.cc',
        'data_encoding_unittest.cc',
        'device_registration_info_unittest.cc',
        'error_unittest.cc',
        'exported_object_manager_unittest.cc',
        'exported_property_set_unittest.cc',
        'http_connection_fake.cc',
        'http_transport_fake.cc',
        'http_utils_unittest.cc',
        'mime_utils_unittest.cc',
        'string_utils_unittest.cc',
        'url_utils_unittest.cc'
      ],
    },
  ],
}
