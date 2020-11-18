pragma solidity >=0.5.0 <6.0.0;

import "./SafeMath.sol";

contract ERC20 {
    // it does not belong to ERC20 standart
    // "restrict non-stop user transactions"
    mapping(address => uint256) _timeCanSendTransaction;
    uint256 _timeBetweenTransaction;
    function _canSendTransaction(address sender) public view returns (bool) {                                                // if free gas - need a fix
        require(_timeCanSendTransaction[sender] <= now, "expect the opportunity to send a transaction");
        return true;
    }
    // 
    
    using SafeMath for uint256;
    
    string internal _name;
    string internal _symbol;
    uint256 internal _decimals;      //uint256 because use in exponentiation
    
    mapping (address => mapping (address => uint256)) internal _allowances;
    mapping (address => uint256) internal _balances;
    uint256 internal _totalSupply;
    
    event Transfer(address indexed sender, address indexed recipient, uint256 value);   // with decimals ???
    event Approval(address indexed owner, address indexed spender, uint256 value);      // with decimals ???
    
    
    function name() public view returns (string memory) {
        return _name;
    }
    function symbol() public view returns (string memory) {
        return _symbol;
    }
    function decimals() public view returns (uint256) {
        return _decimals;
    }
    function totalSupply() public view returns (uint256) {
        return _totalSupply.div(10 ** _decimals);
    }
    function balanceOf(address account) public view returns (uint256) {                              // ???
        return _balances[account].div(10 ** _decimals);
    }
    function allowance(address owner, address spender) public view returns (uint256) {               // ???
        return _allowances[owner][spender].div(10 ** _decimals);
    }
    
    
    
    function transfer(address recipient, uint256 amount) public returns (bool) {
        _canSendTransaction(msg.sender);
        _timeCanSendTransaction[msg.sender] = now + _timeBetweenTransaction;
        
        uint256 amountWithDecimals = amount.mul(10 ** _decimals);
        _transfer(msg.sender, recipient, amountWithDecimals);
        return true;
    }
    function approve(address spender, uint256 amount) public returns (bool) {
        _canSendTransaction(msg.sender);
        _timeCanSendTransaction[msg.sender] = now + _timeBetweenTransaction;
        
        uint256 amountWithDecimals = amount.mul(10 **_decimals);
        _approve(msg.sender, spender, amountWithDecimals);
        return true;
    }
    function transferFrom(address sender, address recipient, uint256 amount) public returns (bool) {
        _canSendTransaction(msg.sender);
        _timeCanSendTransaction[msg.sender] = now + _timeBetweenTransaction;
        
        uint256 amountWithDecimals = amount.mul(10 ** _decimals);
        require(_allowances[sender][msg.sender] >= amountWithDecimals,  "ERC20: transfer amount exceeds allowance");
        _transfer(sender, recipient, amountWithDecimals);
        _approve(sender, msg.sender, _allowances[sender][msg.sender] -= amountWithDecimals);
        return true;
    }
    
    
    
    function _transfer(address sender, address recipient, uint256 amount) internal {                // already with decimals
        require(sender != address(0), "ERC20: transfer from the zero address");
        require(recipient != address(0), "ERC20: transfer to the zero address");
        require(amount <= _balances[sender], "ERC20: transfer amount exceeds balance");
        _balances[sender] -= amount;
        _balances[recipient] += amount;
        emit Transfer(sender, recipient, amount);
    }
    function _approve(address owner, address spender, uint256 amount) internal {                    // already with decimals
        require(owner != address(0), "ERC20: approve from the zero address");
        require(spender != address(0), "ERC20: approve to the zero address");
        _allowances[owner][spender] = amount;
        emit Approval(owner, spender, amount);
    }
}