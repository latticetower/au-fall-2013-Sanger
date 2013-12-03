#
require 'green_shoes'

Shoes.app(title: 'Sanger Megaprocessor', width: 300, height:100) do 
  flow do 
    @e1 = edit_line 'click the button'
    button 'Select folder 1' do
      @e1.text = ask_open_folder
    end
  end
  flow do 
    @e2 = edit_line 'click the button'
    button 'Select folder 2' do
      @e2.text = ask_open_folder
    end
  end
  button 'process' do 
    #here we call module\class with all the magic
  end
end