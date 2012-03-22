//@HEADER
/*
 *******************************************************************************

    Copyright (C) 2004, 2005, 2007 EPFL, Politecnico di Milano, INRIA
    Copyright (C) 2010 EPFL, Politecnico di Milano, Emory University

    This file is part of LifeV.

    LifeV is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LifeV is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LifeV.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************
 */
//@HEADER

/*!
    @file
    @brief

    @author
    @contributor
    @date
 */

#include "AneuriskNavierStokes.hpp"

namespace LifeV
{

void AneuriskNavierStokes::setProblemData()
{
    Debug() << "[AneuriskNavierStokes::setProblemData]\n";

    // problem_type problem;
    problem_type::setParams( this->M_getPot, *this->M_meshPartitionerPtr->meshPartition(), this->M_commPtr );

    if( this->M_getPot("fluid/problem/avg_reynolds", 350) )
    {

        problem_type::setScaleFactor( this->M_oseenDataPtr->viscosity() / this->M_oseenDataPtr->density(),
                                this->M_getPot("fluid/problem/avg_reynolds", 350) );
        this->M_displayer.leaderPrint("Adjusting the scale factor for the inflow BC\n" );
    }
    this->M_displayer.leaderPrint("Scale factor for the inflow BC: ", problem_type::_rescale, "\n" );

    if(this->M_verbose)
        problem_type::showMe();

}


void AneuriskNavierStokes::setBC()
{
    Debug() << "[AneuriskNavierStokes::setBC]\n";

    BloodFlowNavierStokes::setBC();

    //=======================================================================//
    // Boundary Conditions                                                   //
    //=======================================================================//
    BCFunctionBase uWall( problem_type::u_w  );
    //BCFunctionBase u_in (problem_type::u_in );
    M_u_in.setFunction( problem_type::u_in );
    BCFunctionBase g_in( problem_type::g_in );
    BCFunctionBase g_out( gZero );

    for(std::list<UInt>::iterator it=this->M_wall_id.begin();it!=this->M_wall_id.end();++it)
    {
        this->M_bcH.addBC("Wall", *it, Essential,  Full, uWall, nDimensions );
        this->M_displayer.leaderPrint("WALL BC: label ", *it, "\n");
    }
    for(std::list<UInt>::iterator it=this->M_inflow_id.begin();it!=this->M_inflow_id.end();++it)
    {
        this->M_bcH.addBC("Inflow", *it, Essential,  Full, M_u_in, nDimensions );
        this->M_displayer.leaderPrint("INFLOW BC: label ", *it, "\n");
    }
    for(std::list<UInt>::iterator it=this->M_outflow_id.begin();it!=this->M_outflow_id.end();++it)
    {
        this->M_bcH.addBC("Outflow", *it, Natural,  Full, g_out, nDimensions );
        this->M_displayer.leaderPrint("OUTFLOW BC: label ", *it, "\n");
    }
}


void AneuriskNavierStokes::initializeSolution()
{
    Debug() << "[AneuriskNavierStokes::initializeSolutions]\n";

    //=======================================================================//
    // Initialize                                                            //
    //=======================================================================//
    M_bdf_u.setInitialCondition(problem_type::u0, *M_velocityPtr, *M_uFeSpacePtr,
                                M_dataTimePtr->initialTime(), M_dataTimePtr->timeStep());
    M_bdf_p.setInitialCondition(problem_type::p0, *M_pressurePtr, *M_pFeSpacePtr,
                                M_dataTimePtr->initialTime(), M_dataTimePtr->timeStep());

    *M_repeatedConvectiveTermPtr *= 0;
    M_oseenHandlerPtr->assembler().addConvectionRhs(*M_repeatedConvectiveTermPtr,this->M_oseenDataPtr->density(),*M_velocityPtr);
    M_bdf_beta.setInitialCondition(*M_repeatedConvectiveTermPtr, M_dataTimePtr->timeStep());

}


void AneuriskNavierStokes::correctInflow( const Real& t, const std::vector<Real>& currentInflow )
{
    Debug() << "[AneuriskNavierStokes::correctInflow]\n";

    // Monitoring the inflow datum
    Real _correction_ratio( 1 );
    if( std::fabs( *currentInflow.begin() ) )
        _correction_ratio = std::fabs( problem_type::calc_g( t ) ) /
        std::fabs( *currentInflow.begin() );

    std::ostringstream oStream;
    oStream << "\n[main] the actual flow rate is " << *currentInflow.begin()
            << ", I wanted it to be " << -problem_type::calc_g( t )
            << ".\nI'll correct with factor " << _correction_ratio
            << ".\nThe multiplier was " << problem_type::_correction << std::flush;
    problem_type::_correction *= _correction_ratio;
    oStream << ", now it is " << problem_type::_correction << ".\n" << std::endl;
    this->M_displayer.leaderPrint( oStream.str() );

    this->M_u_in.setFunction( problem_type::u_in );
    this->M_bcH.modifyBC( *M_inflow_id.begin(), M_u_in );

}

} // namespace LifeV
