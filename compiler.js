const editorIn = ace.edit("editor-in");
const editorOut = ace.edit("editor-out");

const compile = (evt) => {
  evt.preventDefault();
  evt.stopPropagation();
  const codeInC = editorIn.getValue();

  $.ajax({
    url: 'http://18.228.220.249:3000/compile',
    type: 'POST',
    data: {code: codeInC},
    success: (response) => {
      editorOut.setValue(response.code, 1);
    },
    error: () => {
      console.log('Request failed');
    },
  });
};

$(() => {
  editorIn.setTheme("ace/theme/xcode");
  editorIn.session.setMode("ace/mode/c_cpp");
  editorIn.setOptions({
    enableBasicAutocompletion: true, // the editor completes the statement when you hit Ctrl + Space
    enableLiveAutocompletion: true, // the editor completes the statement while you are typing
    showPrintMargin: true, // hides the vertical limiting strip
    fontSize: "100%" // ensures that the editor fits in the environment
  });

  editorOut.setTheme("ace/theme/monokai");
  editorOut.session.setMode("ace/mode/c_cpp");
  editorOut.setReadOnly(true);
  editorOut.renderer.$cursorLayer.element.style.display = "none"
  editorOut.setOptions({
    enableBasicAutocompletion: true,
    enableLiveAutocompletion: true,
    fontSize: "100%",
    highlightActiveLine: false,
    highlightGutterLine: false,
    readOnly: true,
    showPrintMargin: false
  });
});

$("#btn-compile").click(compile);
