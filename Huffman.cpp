#include "Huffman.h"
#include "Node.h"
#include "istream"
#include "Storage.h"

void Huffman::loadFrequencyTable(std::ifstream& input) { //takes a file stream input
//character that will be used to move through the input stream
char current_character;

//while stream exists
while(input.get(current_character)){
    //if current character does not exist in the frequency table then add it and set it to one, if it does exist then increment it.
    if(frequencyTable.find(current_character) == frequencyTable.end()){
        frequencyTable[current_character] = 1;
    }else{
        frequencyTable[current_character]++;
    }
}
//null terminator
frequencyTable['\x03']++;
}


void Huffman::loadPriorityQueue() {
    //if empty throw error
if(frequencyTable.empty()){
    throw std::invalid_argument("Table Empty");
}
//for all the couples inside the frequency table add each as a node to the priority queue
for(const auto& couple : frequencyTable){
    nodeQueue.push(new Node{couple.first,couple.second});
}
}

void Huffman::buildTree() {
    //created tree by cycling nodes though priority queue
    while(nodeQueue.size() != 1){

        Node* temp1 = nodeQueue.top();
        nodeQueue.pop();


        Node* temp2 = nodeQueue.top();
        nodeQueue.pop();

        Node* created = new Node{'\0',temp1->weight + temp2->weight,temp1,temp2};
        nodeQueue.push(created);
    }
    root = nodeQueue.top();

}



void Huffman::encodeFile(std::ifstream &input, std::string outputFileName) {
    //created storage device
    Storage *storage = new Storage();

    storage->open(outputFileName,"write");
    //sets header, HEADER MUST BE DEFINED PREVIOUSLY FOR THIS TO WORK
    storage->setHeader(header);

    char currentCharacter;
    std::string output;

//for every character in the file, get the characters huffman code equivalent and output the code for it.
    while(input.get(currentCharacter)){

    if(huffmanCodes.find(currentCharacter) == huffmanCodes.end()){
        throw std::invalid_argument("Character does not exist in Codes");
    }

    storage->insert(huffmanCodes[currentCharacter]);
    }

//sentinel character
    storage->insert(huffmanCodes['\x03']);
    storage->close();


    delete storage;
}

void Huffman::loadHuffmanCodes() {
    //calls helper function which gathers the huffman codes using the created binary tree
    loadHuffmanCodeHelper(root, "");

}

void Huffman::loadHuffmanCodeHelper(Node *root, std::string code) {
//if root is nullptr return
if(root == nullptr){
    return;
}

//if we have reached a leaf used the gathered code and input it into the huffman codes using the letter as they key
if(root->zero == nullptr && root->one == nullptr){
    huffmanCodes[root->letter] = code;
    return;
}
//continue traversal adding a 1 or 0 to the code depending on how we traverse the tree
    loadHuffmanCodeHelper(root->one,code + "1");
    loadHuffmanCodeHelper(root->zero,code + "0");
}





void Huffman::buildHeader() {
    //builds the header so it can be retrieved later
if(huffmanCodes.empty()){
    throw std::invalid_argument("Map is empty");
}

std::string holder = "";
for(const auto couple: huffmanCodes){
    holder += couple.first;
    holder += ":";
    holder += couple.second;
    holder += "/";
}
header = holder;

}



void Huffman::loadFetchedHuffmanMap(std::string header) {
    //loads the header from a binary file
    //char set at the first element of the header string
const char* current = header.c_str();
//while we are not at the end
//EX HEADER: a:101/b:00/c:111/
while(*current != '\0'){
    std::string code = "";
    char letter = *current;
    current++;
    current++;

    while(*current != '/' && *current != '\0' ){
        code += *current;
        current++;
    }

    fetchedHuffmanMap[letter] = code;

    if(*current == '/'){
        current++;
    }else{
        break;
    }


}
}

void Huffman::recreateRoot() {
//creating new root
    Node* rootNew = new Node('\0',1, nullptr, nullptr);
//for every pair in the fetched map
    for(const auto& couple : fetchedHuffmanMap){
        //save the letter key
        char letter = couple.first;
        //set the code to this string
        std::string code = couple.second;

        //set cur to the new root so we can iterate through the tree
        Node* cur = rootNew;

        //for every number in the code
        for(char number : code){
            //if the number is 0
            if(number == '0'){
                //if the trees 0 child is uninitialized, creat this node and move cur to it
                if(cur->zero == nullptr){
                    cur->zero = new Node('\0', 1, nullptr, nullptr);
                    cur = cur->zero;
                }else{
                    //if the trees 0 is already initialized, simply set cur to it
                    cur = cur->zero;
                }

            }

            if(number == '1'){
                if(cur->one == nullptr){
                    cur->one = new Node('\0',1, nullptr, nullptr);
                    cur = cur->one;
                }else{
                    cur = cur->one;
                }

            }

        }
        //once we have finished moving though the code, simply set the letter to the node cur is on
        cur->letter = letter;
    }
    //save the recreated root.
    fetchedRoot = rootNew;

}

void Huffman::decodeText(Storage &storage, std::string outFileName) {
std::ofstream outFile(outFileName);

Node* current = fetchedRoot;
std::string chunk;

while(storage.extract(chunk)) {


    for (const char digit : chunk) {

        if (current == nullptr) {
            return;
        }

        if (digit == '0') {
            current = current->zero;

        }

        if (digit == '1') {
            current = current->one;

        }


        if (current->zero == nullptr && current->one == nullptr) {
            if (current->letter == '\x03') {
                outFile.close();
                return;
            }

            outFile << current->letter;
            current = fetchedRoot;
        }

    }
}
outFile.close();

}

void Huffman::compress(std::string inFile, std::string outFile) {


    std::ifstream inputFile(inFile);
    if(!inputFile.is_open()){
        std::cerr << "FILE NOT OPEN!";
    }

    loadFrequencyTable(inputFile);

    inputFile.close();

    loadPriorityQueue();

    buildTree();

    loadHuffmanCodes();

    std::ifstream inputFile2(inFile);

    if(!inputFile2.is_open()){
        std::cerr << "FILE NOT OPEN!";
    }

    buildHeader();

    encodeFile(inputFile2, outFile); //saves implicitly

    inputFile2.close();



}

void Huffman::decompress(std::string inputFile, std::string outputFile) {


    Storage *storage = new Storage();

    //implicitly opens file
    if(!storage->open(inputFile,"read")){
        throw std::logic_error("File could not be found");
    }


    loadFetchedHuffmanMap(storage->getHeader());

    recreateRoot();

    decodeText(*storage,outputFile);


}

Huffman::Huffman() {

root = nullptr;
fetchedRoot = nullptr;
header = "";

}

Huffman::~Huffman() {

    if(root != nullptr){
        deleteTree(root);
    }

    if(fetchedRoot != nullptr){
        deleteTree(fetchedRoot);
    }
}

void Huffman::deleteTree(Node *root) {
    if(root == nullptr){
        return;
    }

    deleteTree(root->zero);
    deleteTree(root->one);

    delete root;
}