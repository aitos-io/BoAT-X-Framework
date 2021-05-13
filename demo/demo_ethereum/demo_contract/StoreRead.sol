pragma solidity >=0.4.16 <0.6.0;

contract StoreRead {
    // Uncomment one of following 2 lines according to truffle version
    // address public organizer;
    address payable public organizer;

    bytes32[] eventList;
    uint32[] uint32List;
    string  stringLocal;
    bytes  bytesLocal;


    constructor () public {
        organizer = msg.sender;
    }

    function saveList(bytes32 newEvent) public {
        eventList.push(newEvent);
    }

    function readListLength() public view returns (uint32 length_) {
        length_ = uint32(eventList.length);
    }

    function readListByIndex(uint32 index) public view returns (bytes32 event_) {
        if(eventList.length > index) {
            event_ = eventList[index];
        }
    }

    //!follow function used for variable-length contract-ABI test
    function func1( uint32 paramUint32, bytes memory paramBytes, string memory paramString) public{
        stringLocal = paramString;
        bytesLocal  = paramBytes;
        uint32List.push(paramUint32);
    }

    function func2(bytes memory paramBytes,  uint32 paramUint32, string memory paramString) public{
        stringLocal = paramString;
        bytesLocal  = paramBytes;
        uint32List.push(paramUint32);
    }

    function func3(bytes memory paramBytes, string memory paramString, uint32 paramUint32) public{
        stringLocal = paramString;
        bytesLocal  = paramBytes;
        uint32List.push(paramUint32);
    }

    /*
    function func1(bytes, string, uint32[]) public{

    } */
    //!function used for variable-length contract-ABI test done


    function destroy() public {
        if (msg.sender == organizer) {
            selfdestruct(organizer);
        }
    }
}
