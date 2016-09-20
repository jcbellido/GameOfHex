#include <iostream>

using namespace std;
namespace hw2
{
	class B {
	public:
		int i;
		virtual void print_i() const { cout << i << " inside B" << endl; }

		virtual void print_numerico(int numerico) const { cout << numerico << " numerico inside B" << endl; }
		virtual void print_numerico(double numerico) const { cout << numerico << " double numerico inside B" << endl; }
	};

	class D : public B {
	public:
		void print_i() const { cout << i << " inside D" << endl; }//virtual
		void print_numerico(int numerico) const { cout << numerico << " numerico inside D" << endl; }
	};

	void normal_override()
	{
		B b;
		B* pb = &b;		// point at a B object
		D f;

		f.i = 1 + (b.i = 1);
		pb->print_i();	// callB::print_i()
		pb = &f;		// point at a D object
		pb->print_i();	// call D::print_i()
	}

	void hw2_virtual_override_sampling()
	{
		B b;
		B* pb = &b;
		D d;
		cout << "Using the pointers to access the objects" << endl;
		pb->print_numerico(1);
		pb->print_numerico(1.5);

		pb = &d;
		pb->print_numerico(1);
		pb->print_numerico(1.5);


		// And here is where the lolz begins
		cout << "Using the objects themselves" << endl;
		b.print_numerico(1);
		b.print_numerico(1.5);

		d.print_numerico(1);
		d.print_numerico(1.5);
	}

};