#include "montercarlo_spin_vertex.hpp"


int MonteCarloIntegratorSpinVertex::integrate(void)
{
    int accepted_moves = 0;
	int sum_mag=0;
	int vertex_index;
    for(int vertex_step = 0; vertex_step<_system.Numvertices; vertex_step++)
    {
        vertex_index=_system.Numvertices*(m_rng->drnd());
		//  attempt to move the vertex
        double energy_i = this->ComputeEnergyFromVertex(vertex_index);
        _system.vertices[vertex_index].spin *= -1;
        double energy_f = this->ComputeEnergyFromVertex(vertex_index);
        double delE = energy_f - energy_i;
        accepted_moves++;
		
        if (!(delE<0.0))
        {
			double rnd_numEli=m_rng->drnd();
            if(!( rnd_numEli < exp(-delE/get_temperature())))
            {
                _system.vertices[vertex_index].spin *= -1;
                accepted_moves--;
            } 
        }	
		sum_mag+= _system.vertices[vertex_index].spin;
    }
	

    return(accepted_moves);
}
