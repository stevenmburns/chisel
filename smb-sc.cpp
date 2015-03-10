#include "systemc.h"
#include <iostream>
#include <iomanip>

SC_MODULE(smb_decr) {//SMB before clocks and resets
public:
//SMB before ports
  sc_in<sc_uint<16> > io_a;
  sc_out<sc_uint<16> > io_b;

//SMB before emitDecs
  sc_signal<sc_uint<16> > T0;

//SMB before emitInits

//SMB before emitDefs
  void m0() { io_b = T0; }
  void m1() { T0 = io_a.read() - sc_uint_base( 10, 16); }


//SMB before emitRegs

//SMB before emitConstructor

  SC_CTOR(smb_decr) : io_a("io_a"), io_b("io_b"), T0("T0") {
     SC_METHOD(m0);
     sensitive << T0;
     SC_METHOD(m1);
     sensitive << io_a;
  }

};

SC_MODULE(smb) {//SMB before clocks and resets
  sc_in_clk clk;
  sc_in<bool> reset;
//SMB before ports
  sc_in<sc_uint<16> > io_a;
  sc_out<sc_uint<16> > io_b;

//SMB before emitDecs
  sc_signal<sc_uint<16> > r2;
  sc_signal<sc_uint<16> > T0;
  sc_signal<sc_uint<16> > r;
  sc_signal<sc_uint<16> > T4;
  sc_signal<sc_uint<16> > T1;
  sc_signal<sc_uint<16> > T2;
  sc_signal<bool > T3;
  sc_signal<bool > z;
  sc_signal<sc_uint<16> > s_io_b;

//SMB before emitInits

//SMB before emitDefs

  void m0() { T0 = ( T3) ? r : r2; }
  void m1() { T4 = ( reset) ? sc_uint_base( 0x0, 16) : T1.read(); }
  void m2() { T1 = ( T3) ? T2 : r; }
  void m3() { T2 = r.read() + io_a.read(); }
  void m4() { T3 = z ^ sc_uint_base( 0x1, 1); }
  void m5() { z = io_a == sc_uint_base( 0x0, 16); }
  void m6() { io_b = s_io_b; }

//SMB before emitRegs

  void t0() {
    wait();
    while(1) {
      if ( T3) {
        r2 = r;
      } 
      if ( reset) {
        r = 0x0;
      } else if ( T3) {
        r = T2;
      }
      wait();
    }
  }

//SMB before emitInstantiations

  smb_decr smb_decr0;

//SMB before emitConstructor

  SC_CTOR(smb) : clk("clk"), reset("reset"), io_a("io_a"), io_b("io_b"), r2("r2"), T0("T0"), r("r"), T4("T4"), T1("T1"), T2("T2"), T3("T3"), z("z"), s_io_b("s_io_b"), smb_decr0("smb_decr0") {
// Methods
     SC_METHOD(m0);
     sensitive << T3 << r << r2;
     SC_METHOD(m1);
     sensitive << reset << T1;
     SC_METHOD(m2);
     sensitive << T3 << T2 << r;
     SC_METHOD(m3);
     sensitive << r << io_a;
     SC_METHOD(m4);
     sensitive << z;
     SC_METHOD(m5);
     sensitive << io_a;
     SC_METHOD(m6);
     sensitive << s_io_b;
// Threads
     SC_THREAD(t0);
     sensitive << clk.pos();     
     
// Module connections
     smb_decr0.io_a(r2);
     smb_decr0.io_b(s_io_b);

  }

};

SC_MODULE(tb) {//SMB before clocks and resets
  sc_in_clk clk;
  sc_out<bool> reset;
//SMB before ports
  sc_in<sc_uint<16> > b;
  sc_out<sc_uint<16> > a;

  SC_CTOR(tb) : clk("clk"), reset("reset"), a("a"), b("b") {
// Threads
     SC_THREAD(t0);
     sensitive << clk.pos();     
  }

  void t0() {
    wait();
    reset = 1;
    wait(5);
    reset = 0;
    for( unsigned int i=0; i<30; ++i) {
      a = sc_uint_base( 1, 16);
      wait(1);
    }
    sc_stop();
  }


};

SC_MODULE(mon) {
  sc_in_clk clk;
  sc_in<bool> reset;
  sc_in<sc_uint<16> > a;
  sc_in<sc_uint<16> > b;

  SC_CTOR(mon) : clk("clk"), reset("reset"), a("a"), b("b") {
     SC_METHOD(m);
     sensitive << clk << reset << a << b;
  }

  void m() {
    std::cout << "UNCLOCKED: " << std::setw(8) << sc_time_stamp().to_default_time_units() << std::setw(6) << reset.read() << std::setw(8) << a.read() << std::setw(8) << b.read() << std::setw(8) << clk.read() << std::endl;
  }

};


int sc_main( int argc, char *argv[])
{
  sc_clock clk( "clk", 10.0, SC_NS, 0.5, 10.0, SC_NS);
  sc_signal<bool> reset;

  sc_signal<sc_uint<16> > a;
  sc_signal<sc_uint<16> > b;

  std::cout << "           " << std::setw(8) << "time" << std::setw(6) << "reset" << std::setw(8) << "a" << std::setw(8) << "b" << std::setw(8) << "clk" << std::endl;

  smb smb0("smb0");
  smb0.clk(clk); smb0.reset(reset);
  smb0.io_a(a); smb0.io_b(b);

  tb tb0("tb0");
  tb0.clk(clk); tb0.reset(reset);
  tb0.a(a); tb0.b(b);

  mon mon0("mon0");
  mon0.clk(clk); mon0.reset(reset);
  mon0.a(a); mon0.b(b);

  sc_start();
}

