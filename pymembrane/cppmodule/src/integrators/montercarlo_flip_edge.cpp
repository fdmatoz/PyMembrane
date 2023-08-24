#include "montercarlo_flip_edge.hpp"
#include "../mesh/edge_flip.hpp"

bool MonteCarloIntegratorFlipEdge::edge_flip(int flip_edge_index, bool flip_face_up)
{
    auto can_flip = pymemb::EdgeFlip_lambda(flip_edge_index,
                               flip_face_up,
                               &_system.faces[0],
                              &_system.vertices[0],
                               &_system.edges[0],
                               &_system.halfedges[0],
                               _system.get_box());
    return can_flip;
}

int MonteCarloIntegratorFlipEdge::integrate(void)
{
    int accepted_moves = 0;
    for (int edge_index = 0; edge_index < _system.Numedges; edge_index++)
    {
        if (freezed_edge[_system.edges[edge_index].type] == false)
        {
            double delE = -this->ComputeEnergyFromEdge(edge_index);
            //decide of the flip is possible
            if (this->edge_flip(edge_index, true) == true)
            {
                delE+=this->ComputeEnergyFromEdge(edge_index);
                accepted_moves++;
                if (!(delE < 0.0))
                {
                    if (!(m_rng->drnd() < exp(-delE / get_temperature())))
                    {
                        this->edge_flip(edge_index, true);
                        accepted_moves--;
                    }
                }
            }
         }
    }
    return (accepted_moves);
}
