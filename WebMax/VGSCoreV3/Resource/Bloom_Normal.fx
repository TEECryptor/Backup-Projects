//Author: peachykeen
//A cheap ps1.1 bloom-ish shader

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.1
			def c0,0.45,0.45,0.45,1
			def c1,0.2,0.2,0.2,1
			tex t0  //sample current pixel
			add r0,t0,t0_bx2             
			//mul r0,t0,t0
			//mul r0,r0,t0		//cube this frame
			//mul r1,r0,t0		//^4 to the other buffer
			//add r0,r0,r1		//add them
			//mul r1,r0,c1	//multiply down for ~40%
			//mul_x2 r0,t0,c0	//multiply up for ~100%
			//add r0,r0,r1		//overlay
		};
	}
}


Technique BlackWhite
{
	//¿¨Í¨Ð§¹û
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.2
			def c0, 0.1f, 0.1f, 0.1f, 0.1f
			def c1, 0.8f, 0.8f, 0.8f, 0.8f
			def c2, 0.2f, 0.2f, 0.2f, 1.0f
			def c3, 0.6f, 0.6f, 0.6f, 1.0f
			def c4, 0.9f, 0.9f, 1.0f, 1.0f
			
			texcoord t0
			texcoord t1
			dp3 r1, t0, t1 
			sub t3, r1, c0
			cmp_sat r0, t3, c3, c2
			sub t3, r1, c1
			cmp_sat r0, t3, c4, r0
		};
	}
}
