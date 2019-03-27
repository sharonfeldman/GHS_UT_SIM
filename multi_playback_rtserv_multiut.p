print "Loading VC Harness\n";
load vcast.mod;wait
route component.5 b vCAST_END {quitall}
route component.5 cb;wait
unload vcast.mod
quitall




