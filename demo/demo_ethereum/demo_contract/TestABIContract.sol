pragma solidity >=0.4.16 <0.6.0;

contract TestABIContract {
    // Uncomment one of following 2 lines according to truffle version
    // address public organizer;
    address payable public organizer;

    uint32 countTimes;
    uint32 ui32;
    string s;
    bool b;
    address a;
    int32[] int32List;

    bytes32 b32;

    string  stringLocal;
    bytes  bytesLocal;

    constructor () public {
        organizer = msg.sender;
        countTimes = 0;
    }

    function count()  public {
        countTimes++;
    }
    
    function getCountTimes() public view returns (uint32 output) {
        output = countTimes;
    }

    function setUint32(uint32 input) public {
        ui32 = input;
    }

    function getUint32() public view returns (uint32 output) {
        output = ui32;
    }

    function setString(string memory input) public {
        s = input;
    }

    function getString() public view returns (string memory output) {
        output = s;
    }

    function setBool(bool input) public {
        b = input;
    }

    function getBool() public view returns (bool output) {
        output = b;
    }

    function setByte(bytes32 input) public {
        b32 = input;
    }

    function getByte() public view returns (bytes32 output) {
        output = b32;
    }

    function setAddress(address input) public {
        a = input;
    }

    function getAddress() public view returns (address output) {
        output = a;
    }

    function setIntArray(int32[] memory input) public {
        int32List = input;
    }

    function getIntArray() public view returns (int32[] memory output) {
        output = int32List;
    }

    function destroy() public {
        if (msg.sender == organizer) {
            selfdestruct(organizer);
        }
    }
}