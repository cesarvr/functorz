{
  "targets": [
    {
      "target_name": "serverz",
      "sources": [ "lib/serverz.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
