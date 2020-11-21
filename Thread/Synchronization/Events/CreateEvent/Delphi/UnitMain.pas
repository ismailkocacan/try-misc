unit UnitMain;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

  ThreadHandle: THandle;
  EventHandle: THandle = 0;
  ThreadId: Cardinal;

  EventResult: Cardinal;

implementation

{$R *.dfm}


// TThreadStartRoutine = function(lpThreadParameter: Pointer): Integer stdcall;
function ThreadStartRoutine(lpThreadParameter: Pointer): Integer stdcall;
begin
  OutputDebugString('ThreadStartRoutine is calisiyor.');
  Sleep(5000);
  if not SetEvent(EventHandle) then
   OutputDebugString('Event set edilemedi');
  Result := 0;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  EventHandle := CreateEvent(nil, false, false, '');
  if EventHandle = 0 then
    RaiseLastOSError;

  ThreadHandle := CreateThread(nil, 0, @ThreadStartRoutine, nil, 0, ThreadId);
  if ThreadHandle <> 0 then
  begin
    EventResult := WaitForSingleObject(EventHandle, INFINITE);
    CloseHandle(ThreadHandle);

    if EventResult = WAIT_OBJECT_0  then
     ShowMessage('Event Signal Oldu.');

    if EventResult = WAIT_TIMEOUT  then
     ShowMessage('Wait TimeOut Oldu.');
  end;

  CloseHandle(EventHandle);
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  EventHandle := CreateEvent(nil, false, false, '');
  if EventHandle = 0 then
    RaiseLastOSError;

  ThreadHandle := CreateThread(nil, 0, @ThreadStartRoutine, nil, 0, ThreadId);
  if ThreadHandle <> 0 then
  begin
    EventResult := WaitForSingleObject(EventHandle, 5000);

    CloseHandle(ThreadHandle);

    if EventResult = WAIT_OBJECT_0  then
     ShowMessage('Event Signal Oldu.');

    if EventResult = WAIT_TIMEOUT  then
     ShowMessage('Wait TimeOut Oldu.');
  end;

  CloseHandle(EventHandle);
end;

end.