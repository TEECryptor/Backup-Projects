//Author: peachykeen
//A cheap ps1.1 bloom-ish shader. Extra bright.

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.1
			def c0,0.5,0.5,0.5,1
			def c1,0.2,0.2,0.2,1
			tex t0               //sample current pixel
			mul r0,t0,t0
			mul r0,r0,t0		//cube this frame
			mul r1,r0,t0		//^4 to the other buffer
			add r0,r0,r1		//add them
			mul_x2 r1,r0,c1	//multiply down for ~40%
			mul_x2 r0,t0,c0	//multiply up for ~100%
			add r0,r0,r1		//overlay
		};
	}
}
