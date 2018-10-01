with import <nixpkgs> {};

stdenv.mkDerivation rec {
  name = "mousewarp-${version}";
  version = "0.0.1";

  src = builtins.filterSource (name: type: !lib.hasPrefix "target" (baseNameOf name) && !lib.hasPrefix "result" (baseNameOf name) && name != ".git") ./.;
  buildInputs = [autoconf automake cmake pkgconfig cairo pango xlibs.libX11 xorg.libXcomposite];
  propagatedBuildInputs = [cairo pango xlibs.libX11 xorg.libXcomposite];

  postPatch = ''
    ls -l
    pwd
    cd libaosd
    ./autogen.sh
    ./configure
    make
    ln -s libaosd.so libaosd/libaosd.so.2
    cd -
  '';

  postInstall = ''
    mkdir -p $out/bin
    mkdir -p $out/lib
    cp mousewarp $out/bin
    cp ../libaosd/libaosd/libaosd.so $out/lib/libaosd.so.2
    patchelf --set-rpath $(patchelf --print-rpath $out/bin/mousewarp):$out/lib $out/bin/mousewarp
  '';

  meta = with stdenv.lib; {
    description = "A mouse warping guiding overlay";
    license = with licenses; [ bsd3 ];
    maintainers = [ maintainers.unode ];
    platforms = platforms.all;
  };
}
