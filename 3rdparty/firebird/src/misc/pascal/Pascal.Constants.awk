($1 == "#define" && match($3, /[0-9]/) == 1) {printf "\t%s = ", $2; if (match($3, /0x/) == 1) printf "$%s", substr($3, 3); else printf "byte(%s)", $3; printf ";\n";}
