pragma solidity >=0.5.0 <6.0.0;

contract Ownership {

	address payable internal _owner;

	modifier onlyOwner() {
		require(msg.sender == _owner, "Caller is not owner");
		_;
	}
}