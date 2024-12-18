#include "montercarlo_vertex.hpp"


int MonteCarloIntegratorVertex::integrate(void)
{
    int accepted_moves = 0;
    for(int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
    {

		// The following IF makes sure that we only move either particle-free vertices or the central vertex of a particle:	
		if ((_system.vertices[vertex_index].type==-1) || (_system.vertices[vertex_index].type==vertex_index)) {
		
		
        //attempt to move the vertex
        double dx, dy, dz;
		double orig_pos[_system.Numvertices][3]; 

        if (m_spherical_move)
			{
				double x = _system.vertices[vertex_index].r.x, y =_system.vertices[vertex_index].r.y, z= _system.vertices[vertex_index].r.z;
				double r = sqrt(x * x + y * y + z * z);
				double sigma = sqrt(m_dx * m_dx + m_dy * m_dy + m_dz * m_dz);
				dx = m_rng->gauss_rng(sigma);
				dy = m_rng->gauss_rng(sigma);
				dz = m_rng->gauss_rng(sigma);
				double x_new = x + dx, y_new = y + dy, z_new = z + dz;
				double r_new = sqrt(x_new * x_new + y_new * y_new + z_new * z_new);
				double scale = r / r_new;
				dx = scale * x_new - x;
				dy = scale * y_new - y;
				dz = scale * z_new - z;
			}
			else
			{
				dx = m_dx * (m_rng->drnd() - 0.5);
				dy = m_dy * (m_rng->drnd() - 0.5);
				dz = m_dz * (m_rng->drnd() - 0.5);
			}
			

			double energy_i = this->ComputeEnergyFromVertex(vertex_index);
			
			if (_system.vertices[vertex_index].type==-1) {    //Particle-free vertices move as usual.
				_system.vertices[vertex_index].r.x+= dx;
				_system.vertices[vertex_index].r.y+= dy;
				_system.vertices[vertex_index].r.z+= dz;
			} else if (_system.vertices[vertex_index].type==vertex_index) {  //Vertices located "inside a particle" move together with the particle center, as a solid body.
					for(int vertex_index_ii = 0; vertex_index_ii<_system.Numvertices; vertex_index_ii++) {
							if (_system.vertices[vertex_index_ii].type==vertex_index) {
									//Keep the original positions, to be able to step back, if MC step is declined.
								double x=_system.vertices[vertex_index_ii].r.x;
								double y=_system.vertices[vertex_index_ii].r.y;
								double z=_system.vertices[vertex_index_ii].r.z;
								orig_pos[vertex_index_ii][0]=x;
								orig_pos[vertex_index_ii][1]=y;
								orig_pos[vertex_index_ii][2]=z;
								double x_new=x + dx;
								double y_new=y + dy;
								double z_new=z + dz;
								double dx1 = dx;
								double dy1 = dy;
								double dz1 = dz;

								if (m_spherical_move) {
									double r = sqrt(x * x + y * y + z * z);
									double r_new = sqrt(x_new * x_new + y_new * y_new + z_new * z_new);
									double scale = r / r_new;
									double dx1 = scale * x_new - x;
									double dy1 = scale * y_new - y;
									double dz1 = scale * z_new - z;
								}
								_system.vertices[vertex_index_ii].r.x+=dx1;
								_system.vertices[vertex_index_ii].r.y+=dy1;
								_system.vertices[vertex_index_ii].r.z+=dz1;
							}
								
					}
			}
			double energy_f = this->ComputeEnergyFromVertex(vertex_index);
			double delE = energy_f - energy_i;
		
			accepted_moves++;

			if (!(delE<0.0))
			{
				if (!(m_rng->drnd() < exp(-delE/get_temperature())))
				{
					if (_system.vertices[vertex_index].type==-1) { 
						_system.vertices[vertex_index].r.x-= dx;
						_system.vertices[vertex_index].r.y-= dy;
						_system.vertices[vertex_index].r.z-= dz;
					} else if (_system.vertices[vertex_index].type==vertex_index) {  //Vertices located "inside a particle" move together with the particle center, as a solid body.
						for(int vertex_index_ii = 0; vertex_index_ii<_system.Numvertices; vertex_index_ii++) {
							if (_system.vertices[vertex_index_ii].type==vertex_index) {
								_system.vertices[vertex_index_ii].r.x=orig_pos[vertex_index_ii][0];
								_system.vertices[vertex_index_ii].r.y=orig_pos[vertex_index_ii][1];
								_system.vertices[vertex_index_ii].r.z=orig_pos[vertex_index_ii][2];
							}
						}
					}
				
					accepted_moves--;
				}
			}
		}
    }
	
 	double totE=0;
	for(int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
    {
		totE+=this->ComputeEnergyFromVertex(vertex_index);
	}
    return(accepted_moves); 
}
