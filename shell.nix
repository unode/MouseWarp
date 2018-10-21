with import <nixpkgs> {};

stdenv.mkDerivation rec {
  name = "mousewarp-${version}";
  version = "0.0.1";

  src = builtins.filterSource (name: type: !lib.hasPrefix "target" (baseNameOf name) && !lib.hasPrefix "result" (baseNameOf name) && name != ".git") ./.;
  buildInputs = [autoconf automake cmake pkgconfig cairo libaosd pcre pango xlibs.libX11 xorg.libpthreadstubs xorg.libXcomposite xorg.libXdmcp];
  propagatedBuildInputs = [cairo libaosd pango xlibs.libX11 xorg.libXcomposite];

  meta = with stdenv.lib; {
    description = "A mouse warping guiding overlay";
    license = with licenses; [ bsd3 ];
    maintainers = [ maintainers.unode ];
    platforms = platforms.all;
  };
}
