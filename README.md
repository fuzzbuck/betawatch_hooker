# Betawatch tinkering
This project was made to help understand how the overwatch 1 beta build works under the hood.

recommended to auto inject the built DLL, but should work fine nonetheless


## legend
- [x] - done
- [ ] - planned
- ? - maybe planned


features:
- [x] - decrypt all pages ( decryptPage function courtesy of @cere4l )
- [ ] - hook ws2_32.send & ws2_32.recv
- [ ] - hook interesting JAM functions
- [?] - patch out initial JAM crypto exchange
