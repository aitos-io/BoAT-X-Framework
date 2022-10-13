pragma solidity >=0.4.16 <0.6.0;

contract SimpleStorage {
    uint storedData;
    event Init(address, uint);
    event Set(address, uint);
    event Stored(uint);

    constructor() public {
        storedData = 100;
        emit Init(msg.sender, 100);
    }

    function set(uint x) public {
        emit Stored(x);
        storedData = x;
        emit Set(msg.sender, x);
    }

    function get() public view returns (uint) {
        return storedData;
    }
}