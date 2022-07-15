
! A trivial example of a .spc file for dylp. This tells dylp to always work
! with the full constraint system. Generally this is more efficient if an lp
! will only be solved once.

lpcontrol fullsys true ;

! Information print for current phase.

lpprint major 1 ;
