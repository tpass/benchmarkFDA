#
## a cylinder
#
algebraic3d

# cut cylinder by planes:

solid leftcyl = cylinder ( 0, 0, -0.15; 0, 0, 0.1; 0.006 )
	and plane (0, 0, -0.122685; 0, 0, -1)
	and plane (0, 0, -0.062685; 0, 0, 1);

solid mycone = cone ( 0, 0, -0.06835625; 0.007; 0, 0, -0.03432875; 0.001 )
	and plane (0, 0, -0.062685; 0, 0, -1)
	and plane (0, 0, -0.04; 0, 0, 1);

solid leftpart = leftcyl or mycone;

solid smallcyl = cylinder ( 0, 0, -0.1; 0, 0, 0.5; 0.002 )
	and plane (0, 0, -0.04; 0, 0, -1)
	and plane (0, 0, 0.; 0, 0, 1);

solid leftcenter = leftpart or smallcyl;

solid rightcyl = cylinder ( 0, 0, -0.15; 0, 0, 0.1; 0.006 )
	and plane (0, 0, 0; 0, 0, -1)
	and plane (0, 0, 0.06; 0, 0, 1);

solid benchmarkgeo = leftcenter or rightcyl;

tlo benchmarkgeo;