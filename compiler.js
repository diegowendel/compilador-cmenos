const editorIn = ace.edit('editor-in');
const editorOut = ace.edit('editor-out');

jQuery.fn.extend({
  disable: function (state) {
      return this.each(function () {
          var $this = $(this);
          $this.toggleClass('disabled', state);
      });
  }
});

const limpar = (evt) => {
  evt.preventDefault();
  evt.stopPropagation();
  const resultado = editorOut.getValue();

  if (resultado) {
    editorOut.setValue('');
    $('#btn-limpar').disable(true);
  }
}

const compile = (evt) => {
  evt.preventDefault();
  evt.stopPropagation();
  const codeInC = editorIn.getValue();

  if (codeInC) {
    $('.custom-spinner').css('visibility', 'visible');
    $.ajax({
      url: 'https://whoiy2ra6e.execute-api.sa-east-1.amazonaws.com/dev/compile',
      cache: false,
      method: 'POST',
      contentType : 'application/json',
      data: JSON.stringify({ code: codeInC }),
      success: (response) => {
        console.log('response', response);
        editorOut.setValue(response.message, 1);
        $('#btn-limpar').disable(false);
        $('.custom-spinner').css('visibility', 'hidden');
      },
      error: () => {
        $('.custom-spinner').css('visibility', 'hidden');
        alert('Erro na requisição');
      },
    });
  } else {
    alert('O editor não pode estar vazio!');
  }
};

$(() => {
  editorIn.setTheme('ace/theme/xcode');
  editorIn.session.setMode('ace/mode/c_cpp');
  editorIn.setOptions({
    enableBasicAutocompletion: true, // the editor completes the statement when you hit Ctrl + Space
    enableLiveAutocompletion: true, // the editor completes the statement while you are typing
    showPrintMargin: true, // hides the vertical limiting strip
    fontSize: '100%' // ensures that the editor fits in the environment
  });

  editorOut.setTheme('ace/theme/monokai');
  editorOut.session.setMode('ace/mode/c_cpp');
  editorOut.setReadOnly(true);
  editorOut.renderer.$cursorLayer.element.style.display = 'none'
  editorOut.setOptions({
    enableBasicAutocompletion: true,
    enableLiveAutocompletion: true,
    fontSize: '100%',
    highlightActiveLine: false,
    highlightGutterLine: false,
    readOnly: true,
    showPrintMargin: false
  });
});

$('#btn-compile').click(compile);
$('#btn-limpar').click(limpar);
