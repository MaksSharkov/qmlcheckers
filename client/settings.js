var whiteCellColor = "#cccccc"
var blackCellColor = "brown"
var selectedCellColor = "green"
var playerManColor = "yellow"
var aiManColor = "black"

var columnName = ["a","b","c","d","e","f","g","h"]
function getName(col,row){
    return columnName[col] + row
}

var defaultBoardSize = 8

var debug = false
var debugTextColor = "green"

function isBlack(row,col){
    if (row % 2 == 0)
        return (col % 2) == 0
    else
        return (col % 2) == 1
}

function indexOf(row,col){
    return row*boardSize+col
}
