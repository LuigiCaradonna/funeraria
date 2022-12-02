<?php

namespace App\Http\Controllers;

use App\Models\Decoration;
use Illuminate\Http\Request;
use Inertia\Inertia;

class DecorationController extends Controller
{
    /**
     * Shows the Decoration list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Decoration page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $decorations = Decoration::query();

        if ( $search != "" ) 
        {
            $decorations->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $decorations->orderby($field, $order);
        }

        return Inertia::render('Decorations', [
            'decorations' => $decorations->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Decoration
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Decoration
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Decorations page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('decorations')->with('error', 'Creazione decorazione non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Decoration::create($validated) )
        {
            return redirect()->route('decorations')->with('error', 'Creazione decorazione non riuscita.');
        }

        return redirect()->route('decorations')->with('success', 'Decorazione creata con successo.');
    }

    /**
     * Updates a Decoration given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Decoration
     * @param int $id id of the Decoration to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Decorations page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('decorations')->with('error', 'Decorazione non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $decoration = Decoration::find($id);

        if ( ! $decoration )
        {
            return redirect()->route('decorations')->with('error', 'Decorazione non trovata.');
        }

        $decoration->name = $request->input('name');

        if ( ! $decoration->save() )
            return redirect()->route('decorations')->with('error', 'Modifica decorazione non riuscita.');
            
        return redirect()->route('decorations')->with('success', 'Decorazione modificata con successo.');
    }

    /**
     * Deletes a Decoration given its id
     * 
     * @param int $id id of the Decoration to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Decorations page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('decorations')->with('error', 'Decorazione non trovata.');

        // This is executed only if the validation succeedes
        $decoration = Decoration::find($id);

        if ( ! $decoration )
        {
            return redirect()->route('decorations')->with('error', 'Decorazione non trovata.');
        }

        if ( ! $decoration->delete() )
            return redirect()->route('decorations')->with('error', 'Eliminazione decorazione non riuscita.');
            
        return redirect()->route('decorations')->with('success', 'Decorazione eliminata con successo.');
    }
    
    /**
     * Deletes a set of Decorations whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Decoration to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Decoration page
     */
    public function multiDelete(Request $request)
    {
        if ( Decoration::destroy(collect($request->input('ids'))) )
            return redirect()->route('decorations')->with('success', 'Decorazioni eliminate con successo.');

        return redirect()->route('decorations')->with('error', 'Eliminazione decorazioni non riuscita.');
    }
}
