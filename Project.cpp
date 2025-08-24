#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>
using namespace std;


// ==========================
// Class Node
// Represents a logic node with a name (char) and a boolean value
// ==========================
class Node {

    private:
        char name;   // Name of the node (e.g., A, B, C)
        bool value;  // Boolean value of the node (0 or 1)
    
    public:
        // Default constructor
        Node(){
            this->name  = '\0';
            this->value = false;
        }

        // Construct node with only a name
        Node(char name){
            this->name = name;
        }

        // Construct node with only a value
        Node(bool value){
            this->value = value;
        }

        // Copy constructor
        Node(const Node& node){
            this->name  = node.get_node_name();
            this->value = node.get_node_value();
        }

        // Setter for node value
        void set_node_value(bool value){
            this->value = value;
        }

        // Setter for node name
        void set_node_name(char name){
            this->name = name;
        }

        // Getter for node value
        bool get_node_value()const{
            return this->value;
        }

        // Getter for node name
        char get_node_name()const{
            return this->name;
        }

        // ====================
        // Logic Operations
        // ====================

        Node AND(Node node){
            Node R_Node;
            if(this->value && node.get_node_value())
                R_Node.set_node_value(true);
            else
                R_Node.set_node_value(false);
            return R_Node;                
        }

        Node OR(Node node){
            Node R_Node;
            if(!this->value && !node.get_node_value())
                R_Node.set_node_value(false);
            else
                R_Node.set_node_value(true);
            return R_Node;                
        }

        Node XOR(Node node){
            Node R_Node;
            if((this->value && !node.get_node_value()) || (!this->value && node.get_node_value()))
                R_Node.set_node_value(true);
            else
                R_Node.set_node_value(false);
            return R_Node;                
        }

        // Assignment operator overload
        Node& operator=(const Node& node){
            this->name  = node.get_node_name();
            this->value = node.get_node_value(); 
            return *this;    
        }

        // Destructor
        ~Node(){}
};



// ==========================
// Class Gate
// Represents a logic gate (AND, OR, NOT, etc.)
// Each gate has 2 inputs, 1 output, and a name
// ==========================
class Gate{
    private:
       string gate_name;  // Gate type (AND, OR, XOR, etc.)
       Node   input_1;    // First input node
       Node   input_2;    // Second input node
       Node   output;     // Output node
    public:
       Gate(){}

       // Copy constructor
       Gate(const Gate& gate){
        this->gate_name = gate.get_gate_name();
        this->input_1   = gate.get_input_1();
        this->input_2   = gate.get_input_2();
        this->output    = gate.get_output();
       }

       // Setters
       void set_gate_name(string name){ this->gate_name = name; }
       void set_input_1(bool in_1){ input_1.set_node_value(in_1); }
       void set_input_2(bool in_2){ input_2.set_node_value(in_2); }       

       // Getters
       string get_gate_name()const{ return this->gate_name; }     
       bool get_input_1()const{ return this->input_1.get_node_value(); }            
       bool get_input_2()const{ return this->input_2.get_node_value(); }     
       bool get_output()const{ return this->output.get_node_value(); }     

       Node get_node_1(){ return &input_1; }
       Node get_node_2(){ return &input_2; }

       // Display gate state (inputs + output)
       void display_gate_values(){
        cout<<input_1.get_node_value()<<" ";
        cout<<input_2.get_node_value()<<" ";
        cout<<output.get_node_value() <<"\n";
       }

       // ====================
       // Logic Operations
       // ====================
       void AND(){ output = input_1.AND(input_2); }
       void OR(){ output = input_1.OR(input_2); }
       void XOR(){ output = input_1.XOR(input_2); }
       void NAND(){ Node node = input_1.AND(input_2); output.set_node_value(!node.get_node_value()); }
       void NOT(){ output.set_node_value(!(input_1.get_node_value())); }
       void XNOR(){ Node node = input_1.XOR(input_2); output.set_node_value(!node.get_node_value()); }
       void NOR(){ Node node = input_1.OR(input_2); output.set_node_value(!node.get_node_value()); }

       // Run gate simulation depending on type
       bool SimulateGate(){
        if(gate_name=="AND") AND();
        else if(gate_name=="OR") OR();
        else if(gate_name=="NOT") NOT();
        else if(gate_name=="NAND") NAND();
        else if(gate_name=="XOR") XOR();
        else if(gate_name=="NOR") NOR();
        else if(gate_name=="XNOR") XNOR();
        return  output.get_node_value();
       }
};



// ==========================
// Class Simulator (Singleton)
// Manages all nodes and gates
// Can run the full simulation
// ==========================
class Simulator{

    private:
        vector<Node*> Node_Vptrs; // List of nodes
        vector<Gate*> Gate_Vptrs; // List of gates
        int c=0;                       // Counter for node indexing
        Simulator(){}                  // Private constructor (Singleton)
        Simulator(const Simulator& simulator)=delete; // Disable copy constructor
    public:
        // Singleton object creator
        static Simulator& create_obj(){
            static Simulator obj;
            return obj;
        }

        // Add a node (stored on heap)
        void post_node(const Node& node){
            Node_Vptrs.push_back(new Node(node));
        }

        // Add a gate (stored on heap)
        void post_gate(const Gate& gate){
            Gate_Vptrs.push_back(new Gate(gate));
        }

        // Find node by name
        Node* FindNode(char name){
            for(auto x:Node_Vptrs){
                if( name == (x->get_node_name()) ) return x;
            }
            return nullptr;
        }

        // Check if node with same name already exists
        bool  FindifMatch(char name){
            for(auto x:Node_Vptrs){
                if( name == (x->get_node_name()) ) return true;
            }
            return false;
        }     

        // Synchronize nodes with the same name
        void  SyncNodes(){
            for(auto i:Node_Vptrs){
                for(auto j:Node_Vptrs){
                    if(i->get_node_name()==j->get_node_name())
                           j->set_node_value(i->get_node_value());
                }
            }
        }   
        
        // Run simulation on all gates in order
        void StartSimulate() {
            for (auto gate : Gate_Vptrs) {
                SyncNodes();

                if (gate->get_gate_name() == "NOT") {
                    // Single input gate
                    gate->set_input_1(Node_Vptrs[c]->get_node_value());
                    gate->SimulateGate();
                    Node_Vptrs[c + 1]->set_node_value(gate->get_output());
                    c += 2;
                }
                else {
                    // Two-input gates
                    gate->set_input_1(Node_Vptrs[c]->get_node_value());
                    gate->set_input_2(Node_Vptrs[c + 1]->get_node_value());
                    gate->SimulateGate();
                    Node_Vptrs[c + 2]->set_node_value(gate->get_output());
                    c += 3;
                }
            }
        }

        // Print all unique nodes with values
        void print_all_nodes() {
            unordered_set<char> seen;
            for (auto i : Node_Vptrs) {
                char name = i->get_node_name();
                if (seen.find(name) == seen.end()) {
                    cout << name << ": " << i->get_node_value() << endl;
                    seen.insert(name);
                }
            }
        }

        // Destructor to free heap memory
        ~Simulator(){
            for(int i=0; i<Node_Vptrs.size();i++)
                 delete Node_Vptrs[i];
            for(int i=0; i<Gate_Vptrs.size();i++)
                 delete Gate_Vptrs[i];
        }        
};



// ==========================
// Class GateGenerator
// Used for parsing user input, creating nodes & gates dynamically
// ==========================
class GateGenerator{
    public:
       Gate& create_gate(const string& name){
           Gate* gate = new Gate;
           gate->set_gate_name(name);
           return *gate;
       }

       Node& create_node(const char& name){
        Node* node = new Node;
        node->set_node_name(name);
        return *node;
       }

       // Parse user commands
       void parse_input(Simulator* simulator, string input, bool& all_flag){

        // Handle NOT gate (special: only 2 nodes used)
        if(input == "NOT"){
            for(int i =0; i<2; i++){
                char name;
                cin>>name;
                simulator->post_node(create_node(name));
            }
            simulator->post_gate(create_gate("NOT"));            
        }

        // Handle 2-input gates
        else if(input == "AND" || input == "OR" || input == "NAND" || 
                input == "NOR" || input == "XOR" || input == "XNOR"){

            for(int i =0; i<3; i++){
                char name;
                cin>>name;
                if(simulator->FindifMatch(name)){
                   simulator->post_node(*(simulator->FindNode(name)));
                }
                else{
                   simulator->post_node(create_node(name));
                }
            }                    
            simulator->post_gate(create_gate(input));
        }

        // Run simulation
        else if(input == "SIM"){
           simulator->StartSimulate();
        }

        // Set value of a node
        else if(input == "SET"){
           char name;
           bool value;
           cin>>name;
           cin>>value;
           (simulator->FindNode(name))->set_node_value(value);
        }

        // Print output
        else if(input == "OUT"){
           string print_choice;
           cin>>print_choice;
           if(print_choice=="ALL"){
            simulator->print_all_nodes();
            all_flag = false;
           }
           else{
            cout<<simulator->FindNode(print_choice[0])->get_node_name()<<": ";
            cout<<simulator->FindNode(print_choice[0])->get_node_value()<<endl;
           }
        }
       }
};



// ==========================
// Main Program
// ==========================
int main(){
    Simulator& simulator=Simulator::create_obj();  // Get simulator singleton
    GateGenerator generator;                       // Create gate generator
    string cmd;
    bool all_flag=true;

    while(all_flag){
        cin>>cmd;                         // Take user input
        generator.parse_input(&simulator,cmd,all_flag);        // Parse and execute
    }
    return 0;
}
