
function fallbackCopyTextToClipboard(text) {
  var textArea = document.createElement("textarea");
  textArea.value = text;
  
  // Avoid scrolling to bottom
  textArea.style.top = "0";
  textArea.style.left = "0";
  textArea.style.position = "fixed";

  document.body.appendChild(textArea);
  textArea.focus();
  textArea.select();

  try {
    var successful = document.execCommand('copy');
    var msg = successful ? 'successful' : 'unsuccessful';
    console.log('Fallback: Copying text command was ' + msg);
  } catch (err) {
    console.error('Fallback: Oops, unable to copy', err);
  }

  document.body.removeChild(textArea);
}
function copyTextToClipboard(text) {
  if (!navigator.clipboard) {
    fallbackCopyTextToClipboard(text);
    return;
  }
  navigator.clipboard.writeText(text).then(function() {
    console.log('Async: Copying to clipboard was successful!');
  }, function(err) {
    console.error('Async: Could not copy text: ', err);
  });
}

function test()
{
  var txtv = document.getElementById("txtalldata");
  copyTextToClipboard(txtv.value);
}

function selectkey(){
  var d = new Date();
  var key = d.getFullYear().toString() + d.getMonth().toString() + d.getDate().toString() + "order";
  return key;
}

function seqkey(){
  var d = new Date();
  var key = d.getFullYear().toString() + d.getMonth().toString() + d.getDate().toString() + "seq";
  return key;
}


function cacheToday(){
  var orderkey = selectkey();
  var skey = seqkey();
  var txtv = document.getElementById("txtalldata");
  var selecseq = document.getElementById("selectseq");
  console.log(selecseq);
  console.log("current seq id=" + selecseq.value);
  var items = document.getElementsByClassName("oitem");
  var menuDetail = '';
  var totalprice = 0;
  var selectTitles = [];
  if(items!=null)
  {
    for(var i=0; i<items.length; ++i)
    {
      if(items[i].checked)
      {
        selectTitles.push(items[i].title);
        if(menuDetail.length!=0)
        {
          menuDetail = menuDetail + '+';
        }
        else
        {
          menuDetail = '.';
        }
        menuDetail = menuDetail + items[i].title +'`' + items[i].value +'元';
        totalprice = totalprice + parseInt(items[i].value);
      }
    }
  }
  if(totalprice!=0)
  {
    menuDetail = menuDetail + '=' + totalprice.toString() + '元';
    menuDetail = selecseq.value + menuDetail;
    txtv.value = menuDetail;
  }
  else
  {
    txtv.value='';
  }
  localStorage.setItem(orderkey, JSON.stringify(selectTitles));
  localStorage.setItem(skey, selecseq.value);
}

function restoreToday(){
  var orderkey = selectkey();
  var skey = seqkey();
  var strseq = localStorage.getItem(skey);
  if(strseq==null || strseq.length==0)
  {
    var defaultselect = '1';
    document.getElementById("selectseq").value = defaultselect;
    localStorage.setItem(skey, defaultselect);
  }
  else
  {
    document.getElementById("selectseq").value = strseq;
  }
  var txtv = document.getElementById("txtalldata");
  var selecseq = document.getElementById("selectseq");
  var items = document.getElementsByClassName("oitem");
  var menuDetail = '';
  var totalprice = 0;
  var strselected = localStorage.getItem(orderkey);
 
  var ccc = JSON.parse(strselected);
  var selectTitles = [];
  
  if(items!=null)
  {
    for(var i=0; i<items.length; ++i)
    {
      if(ccc.indexOf(items[i].title)!=-1)
      {
        items[i].checked = true;
      }
      if(items[i].checked)
      {
        selectTitles.push(items[i].title);
        if(menuDetail.length!=0)
        {
          menuDetail = menuDetail + '+';
        }
        else
        {
          menuDetail = '.';
        }
        menuDetail = menuDetail + items[i].title +'`' + items[i].value +'元';
        totalprice = totalprice + parseInt(items[i].value);
      }
    }
  }
  if(totalprice!=0)
  {
    menuDetail = menuDetail + '=' + totalprice.toString() + '元';
    menuDetail = selecseq.value + menuDetail;
    txtv.value = menuDetail;
  }
  else
  {
    txtv.value='';
  }
  
  $ = layui.jquery;
  var layer = layui.layer,
  form = layui.form;
  form.render();
}

layui.use(['form', 'layer', 'jquery' ], function () {
  $ = layui.jquery;
  var layer = layui.layer,
      form = layui.form;
  form.on('checkbox(switchTest)', function (data) {
    cacheToday();
  });

  form.on('select(selectseq)', function (data) {
    cacheToday();
  });

  });
