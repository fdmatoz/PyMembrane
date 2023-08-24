#include "montercarlo_swap_vertex.hpp"


int MonteCarloIntegratorSwapVertex::integrate(void)
{
    int accepted_moves = 0;
    int v1,v2;
    int type1;
    double energy_i,energy_f;
    for(int vertex_index = 0; vertex_index<_system.Numvertices; vertex_index++)
    {
        v1 = _system.Numvertices*m_rng->drnd();
        v2 = _system.Numvertices*m_rng->drnd();
        // if vertices have same type do nothing
        if ((_system.vertices[v1].type != _system.vertices[v2].type) && (freezed_vertex[_system.vertices[v1].type] == false && freezed_vertex[_system.vertices[v2].type] == false))
        {

            energy_i = this->ComputeEnergyFromVertex(v1);
            energy_i+= this->ComputeEnergyFromVertex(v2);
            type1 = _system.vertices[v1].type;
            _system.vertices[v1].type = _system.vertices[v2].type;
            _system.vertices[v2].type = type1;
            energy_f = this->ComputeEnergyFromVertex(v1);
            energy_f+= this->ComputeEnergyFromVertex(v2);
            double delE = energy_f - energy_i;
            accepted_moves++;
            //stochastic tunnelling
            if(stochastic_tunnelling)
                delE = exp(-stochastic_gamma*delE) - exp(-stochastic_gamma*(energy_f-get_minimum_energy()));
            if (!(delE<0.0))
            {
                if(!(m_rng->drnd() < exp(-delE/get_temperature())))
                {
                    _system.vertices[v2].type = _system.vertices[v1].type;
                    _system.vertices[v1].type = type1;
                    accepted_moves--;
                }
            }
        }
    }
    return(accepted_moves);
}
