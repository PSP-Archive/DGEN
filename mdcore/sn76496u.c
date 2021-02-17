{
	DATATYPE *buf = (DATATYPE *)buffer;
	struct SN76496 *R = &sn[chip];

#if 1
	unsigned int out;
	int left, voli, c0, c1, c2, c3 ;

	voli = length * STEP ;

	c0 = R->Count[0] ;
	c1 = R->Count[1] ;
	c2 = R->Count[2] ;
	c3 = R->Count[3] ;

	if( R->Volume[0] == 0 ){ if( c0 <= voli ){ c0 += voli ; } }
	if( R->Volume[1] == 0 ){ if( c1 <= voli ){ c1 += voli ; } }
	if( R->Volume[2] == 0 ){ if( c2 <= voli ){ c2 += voli ; } }
	if( R->Volume[3] == 0 ){ if( c3 <= voli ){ c3 += voli ; } }

	while( length > 0 )
	{
		out  = 0 ;
		left = STEP;

		{
			voli = R->Output[0] ? c0 : 0 ;
			c0 -= left ;
			while( c0 <= 0 )
			{
				c0 += R->Period[0];
				if( c0 > 0 )
				{
					R->Output[0] ^= 1;
					if( R->Output[0] ){ voli += R->Period[0]; }
					break;
				}
				c0   += R->Period[0];
				voli += R->Period[0];
			}
			if( R->Output[0] ){ voli -= c0 ; }
			if( voli ){ out += voli * R->Volume[0] ; }
		}

		{
			voli = R->Output[1] ? c1 : 0 ;
			c1 -= left ;
			while( c1 <= 0 )
			{
				c1 += R->Period[1];
				if( c1 > 0 )
				{
					R->Output[1] ^= 1;
					if( R->Output[1] ){ voli += R->Period[1]; }
					break;
				}
				c1   += R->Period[1];
				voli += R->Period[1];
			}
			if( R->Output[1] ){ voli -= c1 ; }
			if( voli ){ out += voli * R->Volume[1] ; }
		}

		{
			voli = R->Output[2] ? c2 : 0 ;
			c2 -= left ;
			while( c2 <= 0 )
			{
				c2 += R->Period[2];
				if( c2 > 0 )
				{
					R->Output[2] ^= 1;
					if( R->Output[2] ){ voli += R->Period[2]; }
					break;
				}
				c2   += R->Period[2];
				voli += R->Period[2];
			}
			if( R->Output[2] ){ voli -= c2 ; }
			if( voli ){ out += voli * R->Volume[2] ; }
		}

		{
			voli  = 0 ;
			do
			{
				int nextevent;

				nextevent = ( c3 < left ) ? c3 : left ;

				if( R->Output[3] ){ voli += c3 ; }

				c3 -= nextevent;
				if( c3 <= 0 )
				{
					if( R->RNG & 1 ){ R->RNG ^= R->NoiseFB ; }
					R->RNG >>= 1;
					R->Output[3] = R->RNG & 1;
					c3 += R->Period[3];
					if( R->Output[3] ){ voli += R->Period[3] ; }
				}
				if( R->Output[3] ){ voli -= c3 ; }

				left -= nextevent;

			} while ( left > 0 ) ;

			if( voli ){ out += voli * R->Volume[3] ; }
		}

		if( out > MAX_OUTPUT * STEP )
		{
			*(buf++) = ((DATACONV(MAX_OUTPUT * STEP)*3)>>3);
		}
		else
		{
			*(buf++) = ((DATACONV(out)*3)>>3);
		}

		length-- ;
	}

	R->Count[0] = c0 ;
	R->Count[1] = c1 ;
	R->Count[2] = c2 ;
	R->Count[3] = c3 ;
#endif // 0

#if 0
	int i;
	unsigned int out;
	int left, count, voli, op ;

	voli = length * STEP ;

	/*
	for( i=0 ; i < 4 ; i++ )
	{
		if( R->Volume[i] == 0 )
		{
			if (R->Count[i] <= voli ){ R->Count[i] += voli ; }
		}
	}
	*/
	if( R->Volume[0] == 0 ){ if( R->Count[0] <= voli ){ R->Count[0] += voli ; } }
	if( R->Volume[1] == 0 ){ if( R->Count[1] <= voli ){ R->Count[1] += voli ; } }
	if( R->Volume[2] == 0 ){ if( R->Count[2] <= voli ){ R->Count[2] += voli ; } }
	if( R->Volume[3] == 0 ){ if( R->Count[3] <= voli ){ R->Count[3] += voli ; } }

	while( length > 0 )
	{
		out  = 0 ;
		left = STEP;

		for( i = 0; i < 3 ; i++ )
		{
			op    = R->Output[i] ;
			count = R->Count[i] ;
			voli  = op ? count : 0 ;
			count -= left ;

			if( count <= 0 )
			{
				int pd = R->Period[i] ;

				do 
				{
					count += pd ;
					if( count > 0 )
					{
						op ^= 1 ;
						if( op ){ voli += pd ; }
						R->Output[i] = op ;
						break;
					}
					count += pd ;
					voli  += pd ;
				}
				while( count <= 0 ) ;
			}
			if( op ){ voli -= count ; }

			R->Count[i] = count ;

			if( voli ){ out += voli * R->Volume[i] ; }

			/*
			count = R->Count[i] ;
			voli  = R->Output[i] ? count : 0 ;
			count -= left ;

			while( count <= 0 )
			{
				count += R->Period[i] ;
				if( count > 0 )
				{
					R->Output[i] ^= 1 ;
					if( R->Output[i] ){ voli += R->Period[i] ; }
					break;
				}
				count += R->Period[i] ;
				voli  += R->Period[i] ;
			}
			if( R->Output[i] ){ voli -= count ; }

			R->Count[i] = count ;

			if( voli ){ out += voli * R->Volume[i] ; }
			*/
		}

		{
			op    = R->Output[3] ;
			count = R->Count[3]  ;
			voli  = 0 ;
			do
			{
				int ne = ( count < left ) ? count : left ;

				if( op ){ voli += count ; }

				count -= ne ;

				if( count <= 0 )
				{
					if( R->RNG & 1 ){ R->RNG ^= R->NoiseFB ; }
					R->RNG >>= 1 ;
					op = R->RNG & 1 ;
					count += R->Period[3] ;
					if( op ){ voli += R->Period[3] ; }
				}
				if( op ){ voli -= count ; }

				left -= ne ;

			} while ( left > 0 ) ;

			R->Count[3]  = count ;
			R->Output[3] = op ;
				
			out += voli * R->Volume[3] ;

			/*
			count = R->Count[3] ;
			voli  = 0 ;
			do
			{
				int ne = ( count < left ) ? count : left ;

				if( R->Output[3] ){ voli += count ; }

				count -= ne ;

				if( count <= 0 )
				{
					if( R->RNG & 1 ){ R->RNG ^= R->NoiseFB ; }
					R->RNG >>= 1 ;
					R->Output[3] = R->RNG & 1 ;
					count += R->Period[3] ;
					if( R->Output[3] ){ voli += R->Period[3] ; }
				}
				if( R->Output[3] ){ voli -= count ; }

				left -= ne ;

			} while ( left > 0 ) ;

			R->Count[3] = count ;
				
			out += voli * R->Volume[3] ;
			*/
		}

		if( out > MAX_OUTPUT * STEP )
		{
		    *(buf++) = ((DATACONV(MAX_OUTPUT * STEP)*3)>>3);
		}
		else
		{
		    *(buf++) = ((DATACONV(out)*3)>>3);
		}

		length--;
	}
#endif // 0
}
