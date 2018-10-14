with import <nixos> {};

buildEnv {
  name = "mousewarp-includes";
  paths = [
    cairo.dev
    gcc
    glibc.dev
    pango.dev
    pango.dev
    pcre.dev
    xlibs.libX11.dev
    xlibs.xproto
    xorg.libXcomposite.dev
    xorg.libXdmcp.dev
    xorg.libpthreadstubs
  ];
}
