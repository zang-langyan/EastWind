#
# This spec file is read by gfortran when linking.
# It is used to specify the libraries we need to link in, in the right
# order.
#

%rename lib liborig
*lib: %{static-libquadmath:libquadmath.a%s;:-lquadmath}  %(libgcc) %(liborig)