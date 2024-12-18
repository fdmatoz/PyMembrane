#include "potentialHarmonicDye.hpp"



double ComputeEdgeHarmonicSpinEnergy(const real3 r1,
                                 const real3 r2,
                                 const double k,
                                 const double l0)
{
    real3 rij;
    vsub(rij, r2, r1);

    double dr = sqrt(vdot(rij, rij));

    double energy = 0.5 * k * (dr - l0) * (dr - l0);

    return energy;
}

double ComputeEdgeHarmonicSpinEnergy_ferro(const real s1,
                                  const real s2,
                                  const double J)
{
    auto H = -J*s1*s2;
    return H;
}

void ComputeVertexHarmonicSpinEnergy_kernel(const int Numedges,
                                        HE_EdgeProp *edges,
                                        const HE_VertexProp *vertices,
                                        const double *__restrict__ _k,
                                        const double *__restrict__ _l0,
                                        const double *__restrict__ _J)
{
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {
        int type = edges[edge_index].type;

        int v1 = edges[edge_index].i;
        real3 _r1 = vertices[v1].r;
        auto _s1 = vertices[v1].spin;

        int v2 = edges[edge_index].j;
        real3 _r2 = vertices[v2].r;
        auto _s2 = vertices[v2].spin;
		
		auto _vtype1 = vertices[v1].type;
		auto _vtype2 = vertices[v2].type;

        //Harmonic Energy
		//Free vertices have the full interaction. Vertices at the edge of a particle-covered
		//surface area exhibit only half the interaction with their NN free vertices.
		//Vertices covered by a particle do not interact between themselves.
		double energy;
		if ((_vtype1==-1) && (_vtype2==-1))
			energy = ComputeEdgeHarmonicSpinEnergy(_r1, _r2, _k[type], _l0[type]); 
		if (((_vtype1==-1) && (_vtype2>=0)) || ((_vtype1>=0) && (_vtype2==-1)))
			energy = 0.5*ComputeEdgeHarmonicSpinEnergy(_r1, _r2, _k[type], _l0[type]); 


		//COMMENTED FERRO ENERGY (no spins in particle-on-drop systems)
        //Ferromagnetic Energy
        //energy+=ComputeEdgeHarmonicSpinEnergy_ferro(_s1, _s2, _J[type]);

        ///ADD ENERGY TO THAT EDGE
        edges[edge_index].energy += energy;
    }
	
/* 	double ener_tot;
	for (int edge_index = 0; edge_index < Numedges; edge_index++) {
		ener_tot+=edges[edge_index].energy;
	}
	std::cout << "Total: " << ener_tot << std::endl; */
	
}

double ComputeVertexHarmonicSpinEnergy::compute_edge_energy(int query_edge_index)
{
    int type = _system.edges[query_edge_index].type;
    int v1 = _system.edges[query_edge_index].i;
    real3 _r1 = _system.vertices[v1].r;
    auto _s1 = _system.vertices[v1].spin;

    int v2 = _system.edges[query_edge_index].j;
    real3 _r2 = _system.vertices[v2].r;
    auto _s2 = _system.vertices[v2].spin;

    //Harmonic Energy
    double edge_energy = ComputeEdgeHarmonicSpinEnergy(_r1, _r2, k[type], l0[type]);
    
    //Ferromagnetic Energy
    edge_energy+=ComputeEdgeHarmonicSpinEnergy_ferro(_s1, _s2, J[type]);
    return edge_energy;
}

double ComputeVertexHarmonicSpinEnergy::compute_vertex_energy(int query_vertex_index)
{

    double harmonic_energy = 0.0;
    //double ferro_energy = 0.0;  
    ///< get the triangle that this vertex is part of
    int he = _system.vertices[query_vertex_index]._hedge;
    int first = he;
	
	if (_system.vertices[query_vertex_index].type==-1) {    //ELI: If we deal here with the energy of a particle-free vertex, do the usual calculation.
		do
		{
			// DO SOMETHING WITH THAT FACE
			int edge_index = _system.halfedges[he].edge;
			int type = _system.edges[edge_index].type;
			int v1 = _system.edges[edge_index].i;
			int v2 = _system.edges[edge_index].j;

			real3 _r1 = _system.vertices[v1].r;
			real3 _r2 = _system.vertices[v2].r;
			
			//Vertices covered by a particle do not exhibit harmonic energy.
			//Vertices at the edge of a particle-covered area have 1/2 interaction with their 
			//free NN vertices.
			auto _vtype1 = _system.vertices[v1].type;
			auto _vtype2 = _system.vertices[v2].type;

			if ((_vtype1==-1) && (_vtype2==-1))
				harmonic_energy += ComputeEdgeHarmonicSpinEnergy(_r1, _r2, k[type], l0[type]); //ELI: Originally, there was a 0.5 factor here. I removed it.
			if (((_vtype1==-1) && (_vtype2>=0)) || ((_vtype1>=0) && (_vtype2==-1)))
				harmonic_energy += 0.5*ComputeEdgeHarmonicSpinEnergy(_r1, _r2, k[type], l0[type]); 
			// Note: if ((_vtype1>=0) && (_vtype2>=0)) do nothing, as this is a spring "covered by a colloidal particle".
		
				

			//ferro_energy += 0.5*ComputeEdgeHarmonicSpinEnergy_ferro(_s1, _s2, J[type]);
			
			// MOVE TO THE NEXT FACE
			int he_pair = _system.halfedges[he].pair;
			he = _system.halfedges[he_pair].next;
		} while ((he != first));
	} else if (_system.vertices[query_vertex_index].type==query_vertex_index) {   //ELI: if we deal with a vertex at the center of a particle. Check the energy of this particle's edge.
		int edge_vert_ii;
		edge_vert_ii=_system.vertices[query_vertex_index].mass;
		do
		{	
			he = _system.vertices[edge_vert_ii]._hedge;
			first = he;
			do
			{
				// DO SOMETHING WITH THAT FACE
				int edge_index = _system.halfedges[he].edge;
				int type = _system.edges[edge_index].type;
				int v1 = _system.edges[edge_index].i;
				int v2 = _system.edges[edge_index].j;

				real3 _r1 = _system.vertices[v1].r;
				real3 _r2 = _system.vertices[v2].r;
				
				auto _vtype1 = _system.vertices[v1].type;
				auto _vtype2 = _system.vertices[v2].type;

				if ((_vtype1==-1) && (_vtype2==-1))
					harmonic_energy += ComputeEdgeHarmonicSpinEnergy(_r1, _r2, k[type], l0[type]); 
				if (((_vtype1==-1) && (_vtype2>=0)) || ((_vtype1>=0) && (_vtype2==-1)))
					harmonic_energy += 0.5*ComputeEdgeHarmonicSpinEnergy(_r1, _r2, k[type], l0[type]); 
			
				// MOVE TO THE NEXT FACE
				int he_pair = _system.halfedges[he].pair;
				he = _system.halfedges[he_pair].next;
			} while ((he != first));
			edge_vert_ii=_system.vertices[edge_vert_ii].mass;
		} while (edge_vert_ii!=-1);
	
	}
	
	
// This block is for 
/*     if(_system.vertices[query_vertex_index].spin>0.0)
    {
        auto vertex_type = _system.vertices[query_vertex_index].type;
        harmonic_energy = Ea[vertex_type];
    } */
	
    auto energy = harmonic_energy; // COMMENTED HERE: + ferro_energy;
    return energy;
}

void ComputeVertexHarmonicSpinEnergy::compute_energy(void)
{

    ComputeVertexHarmonicSpinEnergy_kernel(_system.Numedges,
                                       &_system.edges[0],
                                       &_system.vertices[0],
                                       &k[0],
                                       &l0[0],
                                       &J[0]);
}

double ComputeVertexHarmonicSpinForce(const real3 rij,
                                  const double k,
                                  const double l0)
{
    double dr = sqrt(vdot(rij, rij));
    double fval = k * (dr - l0) / dr;
    return fval;
}

void ComputeVertexHarmonicSpinForce_kernel(const int Numedges,
                                       HE_VertexProp *vertices,
                                       const HE_EdgeProp *__restrict__ edges,
                                       const double *__restrict__ _k,
                                       const double *__restrict__ _l0)
{
    //int i = blockIdx.x*blockDim.x + threadIdx.x;
    //if(  i < Numvertices )
    for (int edge_index = 0; edge_index < Numedges; edge_index++)
    {

        int v1 = edges[edge_index].i;
        real3 _r1 = vertices[v1].r;

        int v2 = edges[edge_index].j;
        real3 _r2 = vertices[v2].r;

        real3 _rij;
        vsub(_rij, _r2, _r1);

        int type = edges[edge_index].type;

        double fval = ComputeVertexHarmonicSpinForce(_rij, _k[type], _l0[type]);

        vertices[v1].forceC.x += fval * _rij.x;
        vertices[v1].forceC.y += fval * _rij.y;
        vertices[v1].forceC.z += fval * _rij.z;

        vertices[v2].forceC.x += -1.0 * fval * _rij.x;
        vertices[v2].forceC.y += -1.0 * fval * _rij.y;
        vertices[v2].forceC.z += -1.0 * fval * _rij.z;
    }
}

void ComputeVertexHarmonicSpinEnergy::compute(void)
{

    ComputeVertexHarmonicSpinForce_kernel(_system.Numedges,
                                      &_system.vertices[0],
                                      &_system.edges[0],
                                      &k[0],
                                      &l0[0]);
}

