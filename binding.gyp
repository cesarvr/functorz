{
  "targets": [
    {
      "target_name": "serverz",
      "sources": [ "src/serverz.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src/include/" 

        ],

        "conditions": [
            ['OS=="mac"', {
                    "cflags": ["-std=c++11x", "-Wall", "-pedantic", "-fexceptions"],

                    "defines": [
                        "__MACOSX_CORE__"
                    ],

                    "xcode_settings": {
                        "OTHER_CPLUSPLUSFLAGS": ["-std=c++11", "-stdlib=libc++"],
                        "OTHER_LDFLAGS": ["-stdlib=libc++"],
                        "MACOSX_DEPLOYMENT_TARGET": "10.7",
                        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                    }
                },

                "OS=='linux'", {

                    "cflags": ['-std=c++11', "-Wall", "-fexceptions" ],

                    'include_dirs': [
                        "jdk/include/",
                        "jdk/linux"
                    ],

                }
            ]
        ]
    }
  ]
}
