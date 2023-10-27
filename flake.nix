{

  outputs = {nixpkgs, self}:
  {
    devShells.x86_64-linux = let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
    in {
      default = pkgs.mkShell {
        #NIX_CFLAGS_COMPILE="-std=c++17";
        #buildInputs = [pkgs.cimg pkgs.clang pkgs.ccls];
      };
    };
  };
}
