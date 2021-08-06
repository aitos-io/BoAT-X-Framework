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
        if (eventList.length > index) {
            event_ = eventList[index];
        }
    }

    function destroy() public {
        if (msg.sender == organizer) {
            selfdestruct(organizer);
        }
    }
}
