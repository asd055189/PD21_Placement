#include "GlobalPlacer.h"
#include "ExampleFunction.h"
#include "NumericalOptimizer.h"

GlobalPlacer::GlobalPlacer(Placement &placement)
	:_placement(placement)
{

}


void GlobalPlacer::_random(vector<double> &x){
    double left=_placement.boundryLeft(),right=_placement.boundryRight();
    double top=_placement.boundryTop(),buttom=_placement.boundryBottom();
    for (int i = 0; i < _placement.numModules(); ++i){ 
		x[i*2]=rand()%int(right-left);//x
		x[i*2 + 1]=rand()%int(top-buttom);//y
        if(x[i*2]>right)
            x[i*2]=left+x[i*2]-right;
        if(x[i*2+1]>top)
            x[i*2]=buttom+x[i*2]-top;
        if(x[i*2]+_placement.module(i).width()>right)
            x[i*2]=right-_placement.module(i).width();
        if(x[i*2+1]+_placement.module(i).height()>top)
            x[i*2+1]=top-_placement.module(i).height();
	    _placement.module(i).setPosition(x[i*2], x[i*2 + 1]);
	}
}
void GlobalPlacer::place()
{
	///////////////////////////////////////////////////////////////////
	// The following example is only for analytical methods.
	// if you use other methods, you can skip and delete it directly.
	//////////////////////////////////////////////////////////////////

	//ExampleFunction ef; // require to define the object function and gradient function

    vector<double> x; // solution vector, size: num_blocks*2 
                         // each 2 variables represent the X and Y dimensions of a block
    //x[0] = 100; // initialize the solution vector
    //x[1] = 100;

    srand(time(NULL));
    x.resize(_placement.numModules() * 2);
    _random(x);
   /* NumericalOptimizer no(ef);
    no.setX(x); // set initial solution
    no.setNumIteration(35); // user-specified parameter
    no.setStepSizeBound(5); // user-specified parameter
    no.solve(); // Conjugate Gradient solver*/

    /*
    cout << "Current solution:" << endl;
    for (unsigned i = 0; i < no.dimension(); i++) {
        cout << "x[" << i << "] = " << no.x(i) << endl;
    }
    cout << "Objective: " << no.objective() << endl;*/
	////////////////////////////////////////////////////////////////


}


void GlobalPlacer::plotPlacementResult( const string outfilename, bool isPrompt )
{
    ofstream outfile( outfilename.c_str() , ios::out );
    outfile << " " << endl;
    outfile << "set title \"wirelength = " << _placement.computeHpwl() << "\"" << endl;
    outfile << "set size ratio 1" << endl;
    outfile << "set nokey" << endl << endl;
    outfile << "plot[:][:] '-' w l lt 3 lw 2, '-' w l lt 1" << endl << endl;
    outfile << "# bounding box" << endl;
    plotBoxPLT( outfile, _placement.boundryLeft(), _placement.boundryBottom(), _placement.boundryRight(), _placement.boundryTop() );
    outfile << "EOF" << endl;
    outfile << "# modules" << endl << "0.00, 0.00" << endl << endl;
    for( size_t i = 0; i < _placement.numModules(); ++i ){
        Module &module = _placement.module(i);
        plotBoxPLT( outfile, module.x(), module.y(), module.x() + module.width(), module.y() + module.height() );
    }
    outfile << "EOF" << endl;
    outfile << "pause -1 'Press any key to close.'" << endl;
    outfile.close();

    if( isPrompt ){
        char cmd[ 200 ];
        sprintf( cmd, "gnuplot %s", outfilename.c_str() );
        if( !system( cmd ) ) { cout << "Fail to execute: \"" << cmd << "\"." << endl; }
    }
}

void GlobalPlacer::plotBoxPLT( ofstream& stream, double x1, double y1, double x2, double y2 )
{
    stream << x1 << ", " << y1 << endl << x2 << ", " << y1 << endl
           << x2 << ", " << y2 << endl << x1 << ", " << y2 << endl
           << x1 << ", " << y1 << endl << endl;
}
