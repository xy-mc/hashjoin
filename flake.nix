{
  description = "Kmer counter";

  inputs = {
    mars-std.url = "github:mars-research/mars-std";
  };

  outputs = { self, mars-std, ... }: let
  # System types to support.
  supportedSystems = [ "x86_64-linux" ];

  in mars-std.lib.eachSystem supportedSystems (system: let
    pkgs = mars-std.legacyPackages.${system};
    in rec {
      devShells.prod = pkgs.mkShell {
        buildInputs = with pkgs; [
        ];
        propagatedBuildInputs = with pkgs; [
          numactl
        ];
        NIX_CFLAGS_COMPILE = "-march=native";
      };
      devShells.build = pkgs.mkShell {
        inputsFrom = [
          devShells.prod
        ];
        nativeBuildInputs = with pkgs; [ 
          gcc
        ];
        buildInputs = with pkgs; [
        ];  
        NIX_CFLAGS_COMPILE = "-march=native";
      };
      devShell = pkgs.mkShell {
        inputsFrom = [
          devShells.build
        ];
        buildInputs = with pkgs; [
          gdb
          clang-tools
        ];  
        NIX_CFLAGS_COMPILE = "-march=native";
      };
    }
  );
}
