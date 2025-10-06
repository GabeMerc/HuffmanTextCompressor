#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include "Node.h"
#include "Storage.h"

#ifndef HUFFMAN_H
/**
 * Class used for huffman compression
 */
class Huffman{

private:




//encode
/**
 * stores the root of the binary tree
 */
    Node* root;
/**
 * Stores the frequency table which ties each character to its frequency (number) in the given piece of text.
 */
    std::unordered_map<char,int> frequencyTable;
/**
 * Stores each character and the huffman code it is tied to.
 */
    std::unordered_map<char,std::string> huffmanCodes;

/**
 * Min-Priority queue which is weighted by the frequency of each letter.
 */
    std::priority_queue<Node*,std::vector<Node*>,compareWeights> nodeQueue;
/**
 * The header contains a string version of the map that stores the huffman codes, which is used in the storage class. .
 */
    std::string header;


//decode

/**
 * Huffman map used when decoded, rebuilt from the header when a file is encoded.
 */
    std::unordered_map<char,std::string> fetchedHuffmanMap;
/**
 * a rebuilt huffman tree that mimics the pre-encoded tree, used for decoding.
 */
    Node* fetchedRoot;




public:
/**
* default constructor.
*/

    Huffman();

/**
 * destructor
 */
    ~Huffman();

/**
 * Compress a file, outputting a compressed binary file.
 * @param inputFile txt file that will be compressed.
 * @param outputFile where compressed file will be written to.
 */
    void compress(std::string inputFile, std::string outputFile);
/**
 * decompresses a compressed file, outputs a decompressed txt file.
 * @param inputFile compressed file that will be decompressed.
 * @param outputFile where the decompressed file will be written to.
 */
    void decompress(std::string inputFile, std::string outputFile);


private:
//encode

/**
 *fills the frequency table mapping the entre input file, character : frequency.
 * @param input file
 */
    void loadFrequencyTable(std::ifstream &input);
/**
 * creates and pushes nodes that contain each letter weighted by their frequency into a min priority queue.
 */
    void loadPriorityQueue();
/**
 * builds the huffman tree using the priority queue.
 */
    void buildTree();

/**
 * creates a map that ties each character to its given huffman code, (string character : string code)
 */
    void loadHuffmanCodes();
/**
 * recursive helper to create help gather all the codes and load them into the huffmanCodes map.
 * @param root, each tree and subtree the recursive function starts at
 * @param code, that is generated over each loop
 */
    void loadHuffmanCodeHelper(Node* root, std::string code);
/**
 * uses the huffman map to encode each of the times into compresssed text.
 * @param input , the input stream that is used to get each of the characters from the file
 * @param outputFile, the file that the compressed text will be written to.
 */
    void encodeFile(std::ifstream &input, std::string outputFile);

/**
 * builds a string version of the huffman map so it can be stored and retrieved.
 */
    void buildHeader();

//decode
/**
 * uses the header of the compressed file to rebuild the original huffman map.
 * @param header of the binary file
 */
    void loadFetchedHuffmanMap(std::string header);

/**
 * recreates the huffman tree using the recreated huffman map.
 */
    void recreateRoot();

/**
 * decodes the text and writes it to another file using the rebuilt huffman tree
 * @param storage, that is used to access the file
 * @param outFileName, the file that will be written out to.
 */
    void decodeText( Storage& storage, std::string outFileName);
/**
 * deletes a given tree
 * @param root of the tree
 */
    void deleteTree(Node* root);


};


#define HUFFMAN_H

#endif //HUFFMAN_H